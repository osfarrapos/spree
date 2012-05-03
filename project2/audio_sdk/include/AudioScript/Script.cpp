/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "Script.h"
#include "TokenFile.h"

using namespace std;
using namespace Audio;

typedef stack<ScriptNode*> NodeStack;
// {00000000-0000-0000-0000-000000000000}
static const GUID SCRIPT_NULL_GUID = 
{ 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };


//------------------------------------------------------------------------//
Script::Script()
{
	Clear();
}

//------------------------------------------------------------------------//
Script::~Script()
{
	// We need a stack for non-recursive tree iteration
	NodeStack nodestack;
	
	ScriptNode* pCurrentNode = m_pRoot;
	ScriptNode* pDeleteNode = m_pRoot;
	m_pRoot = 0;

	// Iterate through all nodes and delete them
	while(pCurrentNode)
	{
		// Go down child branch
		if(pCurrentNode->GetChild())
		{
			nodestack.push(pCurrentNode);
			pCurrentNode = pCurrentNode->GetChild();
			pCurrentNode->m_pParent->m_pChild = 0;
			pCurrentNode->m_pParent = 0;
		}
		// Else traverse sibling branch
		else
		{
			pDeleteNode = pCurrentNode;
			pCurrentNode = pCurrentNode->GetSibling();
			if(!pCurrentNode)
			{
				if(!nodestack.empty())
				{
					pCurrentNode = nodestack.top();
					nodestack.pop();
				}
			}
			_XASAFE_DELETE(pDeleteNode);
		}
	}
	Clear();
}

//------------------------------------------------------------------------//
void Script::Clear()
{
	m_TokenList.clear();
	m_pRoot = 0;
}

//------------------------------------------------------------------------//
ScriptNode* Script::GetRoot()
{
	return m_pRoot;
}





ScriptNode::ScriptNode()
{
	Clear();
}

ScriptNode::~ScriptNode()
{
	Clear();
}


void ScriptNode::Clear()
{
	m_pName = 0;
	m_pData = 0;
	m_pParent = 0;
	m_pSibling = 0;
	m_pChild = 0;
}

bool ScriptNode::HasData()
{
	return (GetDataType() != Script::NO_NODE_DATA);
}

const char* ScriptNode::GetName()
{
	assert(m_pName);
	return m_pName->GetVariable();
}


Script::SCRIPT_TYPE ScriptNode::GetDataType()
{
	if(m_pData)
	{
		switch(m_pData->GetType())
		{
		case Token::VARIABLE:
			return Script::VARIABLE;
		case Token::STRING:
			return Script::STRING;
		case Token::INTEGER:
			return Script::INTEGER;
		case Token::REAL:
			return Script::REAL;
		case Token::BOOLEAN:
			return Script::BOOLEAN;
		case Token::VECTOR:
			return Script::VECTOR;
		};
	}
	return Script::NO_NODE_DATA;
}

const char* ScriptNode::GetVariable()
{
	assert(m_pData);
	return m_pData->GetVariable();
}

const char* ScriptNode::GetString()
{
	assert(m_pData);
	return m_pData->GetString();
}

int ScriptNode::GetInteger()
{
	assert(m_pData);
	return m_pData->GetInteger();
}

bool ScriptNode::GetBool()
{
	assert(m_pData);
	return m_pData->GetBoolean();
}

double ScriptNode::GetReal()
{
	assert(m_pData);
	return m_pData->GetReal();
}

AUDIOVECTOR ScriptNode::GetVector()
{
	assert(m_pData);
	AUDIOVECTOR vec;
	TokenVector3D tvec = m_pData->GetVector();
	vec.x = tvec.x;
	vec.y = tvec.y;
	vec.z = tvec.z;
	return vec;
}

GUID ScriptNode::GetGuid()
{
	assert(m_pData);
	return m_pData->GetGuid();
}


ScriptNode* ScriptNode::GetSibling()
{
	return m_pSibling;
}

ScriptNode* ScriptNode::GetChild()
{
	return m_pChild;
}

ScriptNode* ScriptNode::GetParent()
{
	return m_pParent;
}


ScriptLoader::ScriptLoader()
{
	Clear();
}

ScriptLoader::~ScriptLoader()
{
	Clear();
}


void ScriptLoader::Clear()
{
	m_Lexer.Clear();
}


bool ScriptLoader::Init()
{
	m_Lexer.Init();
	m_Lexer.EnableComments(true);
	m_Lexer.ProcessMacros(true);
	return true;
}

void ScriptLoader::Term()
{
	m_Lexer.Term();
}


bool ScriptLoader::Load(string sFileName, Script& script)
{
	// Process the loaded data and build the node tree
	if(!m_Lexer.Process(sFileName, script.m_TokenList))
		return false;
	return BuildNodeTree(script);
}

bool ScriptLoader::Load(uint8* pData, uint32 nDataLength, Script& script)
{
	// Process the loaded data and build the node tree
	if(!m_Lexer.Process(pData, nDataLength, script.m_TokenList))
		return false;
	return BuildNodeTree(script);
}


bool ScriptLoader::BuildNodeTree(Script& script)
{

	DebugOut(4, "Building script tree now");
	enum SCRIPT_BUILD_STATE
	{
		SBS_NEW_NODE,
		SBS_POST_NODE,
		SBS_NODE_VALUE,
		SBS_NODE_TERMINATOR
	};

	SCRIPT_BUILD_STATE eState = SBS_NEW_NODE;

	// An empty script is not currently defined as being illegal.  If
	// this changes, change this to return false.
	if(!script.m_TokenList.size())
	{
		DebugOut(4, "Found empty script file - returning success");
		return true;
	}

	ScriptNode* pCurrentParentNode = 0;
	ScriptNode* pCurrentNode = 0;

	for(TokenList::iterator itr = script.m_TokenList.begin(); 
		itr != script.m_TokenList.end(); ++itr)
	{
		DebugOut(5, "Token: %s", itr->GetDescriptiveString());
		switch(eState)
		{
		// Create a new node in the tree
		case SBS_NEW_NODE:
			{
				// First check for end brackets to end a child set
				if((itr->GetType() == Token::OPERATOR) && 
					(strcmp(itr->GetOperator(), "}") == 0))
				{
					pCurrentNode = pCurrentParentNode;
					if(pCurrentParentNode)
						pCurrentParentNode = pCurrentParentNode->m_pParent;
					else
						pCurrentParentNode = 0;
				}
				// Otherwise create a node
				else
				{
					if(itr->GetType() != Token::VARIABLE)
						return Error::Handle("Script syntax error at token: %s", script.m_TokenList.GetDescriptiveString(itr).c_str());

					// Allocate a root node for the script
					ScriptNode* pNode = new ScriptNode;
					if(!pCurrentNode && !pCurrentParentNode)
					{
						script.m_pRoot = pNode;
						pCurrentNode = pNode;
					}
					// If there is no current node but there is a parent node,
					// create the first child node of the parent
					else if(!pCurrentNode && pCurrentParentNode)
					{
						pCurrentParentNode->m_pChild = pNode;
						pCurrentNode = pNode;
						pCurrentNode->m_pParent = pCurrentParentNode;
					}
					else if(pCurrentNode)
					{
						pCurrentNode->m_pSibling = pNode;
						pNode->m_pParent = pCurrentNode->m_pParent;
						pCurrentNode = pNode;
					}

					// Assign the name to this node using the current token
					pCurrentNode->m_pName = &(*itr);

					// Set the state to look for instructions following
					// the node name
					eState = SBS_POST_NODE;
				}
			}
			break;

		// Look after the node name to see what we'll be doing
		case SBS_POST_NODE:
			{
				// The next token must be an operator
				if(itr->GetType() != Token::OPERATOR)
					return Error::Handle("Script syntax error at token: %s", script.m_TokenList.GetDescriptiveString(itr).c_str());
				// If an equals sign, the next token must be
				// the value
				if(strcmp(itr->GetOperator(), "=") == 0)
					eState = SBS_NODE_VALUE;
				// Otherwise we're creating a set of child nodes
				else if(strcmp(itr->GetOperator(), "{") == 0)
				{
					eState = SBS_NEW_NODE;
					pCurrentParentNode = pCurrentNode;
					pCurrentNode = 0;
				}
				else if(strcmp(itr->GetOperator(), ";") == 0)
					eState = SBS_NEW_NODE;
				// Anything else is an error
				else
				{
					return Error::Handle("Script syntax error at token: %s", script.m_TokenList.GetDescriptiveString(itr).c_str());
				}
			}
			break;

		// We just passed an equal sign, so we know were assigning
		// a value to this node
		case SBS_NODE_VALUE:
			{
				if(itr->GetType() == Token::OPERATOR)
					return Error::Handle("Script syntax error at token: %s", script.m_TokenList.GetDescriptiveString(itr).c_str());
				pCurrentNode->m_pData = &(*itr);
				eState = SBS_NODE_TERMINATOR;
			}
			break;

		// Look at the node terminator to determine if we're going
		// to assign a value or create children
		case SBS_NODE_TERMINATOR:
			{
				if(itr->GetType() != Token::OPERATOR)
					return Error::Handle("Script syntax error at token: %s", script.m_TokenList.GetDescriptiveString(itr).c_str());
				if(strcmp(itr->GetOperator(), ";") == 0)
					eState = SBS_NEW_NODE;
				else if(strcmp(itr->GetOperator(), "{") == 0)
				{
					eState = SBS_NEW_NODE;
					pCurrentParentNode = pCurrentNode;
					pCurrentNode = 0;
				}
				else
				{
					return Error::Handle("Script syntax error at token: %s", script.m_TokenList.GetDescriptiveString(itr).c_str());
				}
			}
			break;
		};
	}

	DebugOut(4, "Successfully built script tree");
	return true;
}

