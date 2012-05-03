# Microsoft Developer Studio Generated NMAKE File, Based on EmperorOfDragons.dsp
!IF "$(CFG)" == ""
CFG=EmperorOfDragons - Win32 Debug
!MESSAGE No configuration specified. Defaulting to EmperorOfDragons - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "EmperorOfDragons - Win32 Release" && "$(CFG)" != "EmperorOfDragons - Win32 Debug" && "$(CFG)" != "EmperorOfDragons - Win32 Release_include_debugging_information" && "$(CFG)" != "EmperorOfDragons - Win32 Release_Manager"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EmperorOfDragons.mak" CFG="EmperorOfDragons - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EmperorOfDragons - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 Release_include_debugging_information" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 Release_Manager" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EmperorOfDragons - Win32 Release"

OUTDIR=.\Compiled/release/EOD
INTDIR=.\Compiled/release/EOD
# Begin Custom Macros
OutDir=.\Compiled/release/EOD
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\Game\EmperorOfDragons.exe" "$(OUTDIR)\EmperorOfDragons.bsc"

!ELSE 

ALL : "XKernel - Win32 Release" "XGamebase - Win32 Release" ".\Game\EmperorOfDragons.exe" "$(OUTDIR)\EmperorOfDragons.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"XGamebase - Win32 ReleaseCLEAN" "XKernel - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\C3TimesEffect.obj"
	-@erase "$(INTDIR)\C3TimesEffect.sbr"
	-@erase "$(INTDIR)\CArmsSignal.obj"
	-@erase "$(INTDIR)\CArmsSignal.sbr"
	-@erase "$(INTDIR)\CEffectBillboard.obj"
	-@erase "$(INTDIR)\CEffectBillboard.sbr"
	-@erase "$(INTDIR)\CEffectBlood.obj"
	-@erase "$(INTDIR)\CEffectBlood.sbr"
	-@erase "$(INTDIR)\CEffectMgr.obj"
	-@erase "$(INTDIR)\CEffectMgr.sbr"
	-@erase "$(INTDIR)\CEffectShadow.obj"
	-@erase "$(INTDIR)\CEffectShadow.sbr"
	-@erase "$(INTDIR)\CFootHold.obj"
	-@erase "$(INTDIR)\CFootHold.sbr"
	-@erase "$(INTDIR)\CircleParticle.obj"
	-@erase "$(INTDIR)\CircleParticle.sbr"
	-@erase "$(INTDIR)\CirclePattern.obj"
	-@erase "$(INTDIR)\CirclePattern.sbr"
	-@erase "$(INTDIR)\CLevUpEffect.obj"
	-@erase "$(INTDIR)\CLevUpEffect.sbr"
	-@erase "$(INTDIR)\CLevUpTypeB.obj"
	-@erase "$(INTDIR)\CLevUpTypeB.sbr"
	-@erase "$(INTDIR)\CObs.obj"
	-@erase "$(INTDIR)\CObs.sbr"
	-@erase "$(INTDIR)\CommonFuncs.obj"
	-@erase "$(INTDIR)\CommonFuncs.sbr"
	-@erase "$(INTDIR)\ControlFunUtil.obj"
	-@erase "$(INTDIR)\ControlFunUtil.sbr"
	-@erase "$(INTDIR)\CsuX3D_Box.obj"
	-@erase "$(INTDIR)\CsuX3D_Box.sbr"
	-@erase "$(INTDIR)\d3dfont.obj"
	-@erase "$(INTDIR)\d3dfont.sbr"
	-@erase "$(INTDIR)\d3dutil.obj"
	-@erase "$(INTDIR)\d3dutil.sbr"
	-@erase "$(INTDIR)\dsutil.obj"
	-@erase "$(INTDIR)\dsutil.sbr"
	-@erase "$(INTDIR)\dxutil.obj"
	-@erase "$(INTDIR)\dxutil.sbr"
	-@erase "$(INTDIR)\EmperorOfDragons.obj"
	-@erase "$(INTDIR)\EmperorOfDragons.pch"
	-@erase "$(INTDIR)\EmperorOfDragons.res"
	-@erase "$(INTDIR)\EmperorOfDragons.sbr"
	-@erase "$(INTDIR)\GenericWindowClass.obj"
	-@erase "$(INTDIR)\GenericWindowClass.sbr"
	-@erase "$(INTDIR)\KUWater.obj"
	-@erase "$(INTDIR)\KUWater.sbr"
	-@erase "$(INTDIR)\MixerBase.obj"
	-@erase "$(INTDIR)\MixerBase.sbr"
	-@erase "$(INTDIR)\MixerFader.obj"
	-@erase "$(INTDIR)\MixerFader.sbr"
	-@erase "$(INTDIR)\MyBitmap.obj"
	-@erase "$(INTDIR)\MyBitmap.sbr"
	-@erase "$(INTDIR)\ParticleBillbord.obj"
	-@erase "$(INTDIR)\ParticleBillbord.sbr"
	-@erase "$(INTDIR)\ParticleEmitter.obj"
	-@erase "$(INTDIR)\ParticleEmitter.sbr"
	-@erase "$(INTDIR)\ParticleMgr.obj"
	-@erase "$(INTDIR)\ParticleMgr.sbr"
	-@erase "$(INTDIR)\Particlesys.obj"
	-@erase "$(INTDIR)\Particlesys.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\sucollide.obj"
	-@erase "$(INTDIR)\sucollide.sbr"
	-@erase "$(INTDIR)\sumath.obj"
	-@erase "$(INTDIR)\sumath.sbr"
	-@erase "$(INTDIR)\SuMixerVolume.obj"
	-@erase "$(INTDIR)\SuMixerVolume.sbr"
	-@erase "$(INTDIR)\SuMP3Play.obj"
	-@erase "$(INTDIR)\SuMP3Play.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\X3DObject.obj"
	-@erase "$(INTDIR)\X3DObject.sbr"
	-@erase "$(INTDIR)\X3DObjectManager.obj"
	-@erase "$(INTDIR)\X3DObjectManager.sbr"
	-@erase "$(INTDIR)\X3DPickedPrimitive.obj"
	-@erase "$(INTDIR)\X3DPickedPrimitive.sbr"
	-@erase "$(INTDIR)\X3DUFont.obj"
	-@erase "$(INTDIR)\X3DUFont.sbr"
	-@erase "$(INTDIR)\X3PCamera.obj"
	-@erase "$(INTDIR)\X3PCamera.sbr"
	-@erase "$(INTDIR)\XAnimationController.obj"
	-@erase "$(INTDIR)\XAnimationController.sbr"
	-@erase "$(INTDIR)\XBallGauge.obj"
	-@erase "$(INTDIR)\XBallGauge.sbr"
	-@erase "$(INTDIR)\XBillboard.obj"
	-@erase "$(INTDIR)\XBillboard.sbr"
	-@erase "$(INTDIR)\XBipedController.obj"
	-@erase "$(INTDIR)\XBipedController.sbr"
	-@erase "$(INTDIR)\XButton.obj"
	-@erase "$(INTDIR)\XButton.sbr"
	-@erase "$(INTDIR)\XChatEdit.obj"
	-@erase "$(INTDIR)\XChatEdit.sbr"
	-@erase "$(INTDIR)\XCheckButton.obj"
	-@erase "$(INTDIR)\XCheckButton.sbr"
	-@erase "$(INTDIR)\XCompress.obj"
	-@erase "$(INTDIR)\XCompress.sbr"
	-@erase "$(INTDIR)\XDprintf.obj"
	-@erase "$(INTDIR)\XDprintf.sbr"
	-@erase "$(INTDIR)\XFastCode.obj"
	-@erase "$(INTDIR)\XFastCode.sbr"
	-@erase "$(INTDIR)\XGUIControlCenter.obj"
	-@erase "$(INTDIR)\XGUIControlCenter.sbr"
	-@erase "$(INTDIR)\XGUIObject.obj"
	-@erase "$(INTDIR)\XGUIObject.sbr"
	-@erase "$(INTDIR)\XHSlider.obj"
	-@erase "$(INTDIR)\XHSlider.sbr"
	-@erase "$(INTDIR)\XImageStatic.obj"
	-@erase "$(INTDIR)\XImageStatic.sbr"
	-@erase "$(INTDIR)\Xitem.obj"
	-@erase "$(INTDIR)\Xitem.sbr"
	-@erase "$(INTDIR)\XLensFlare.obj"
	-@erase "$(INTDIR)\XLensFlare.sbr"
	-@erase "$(INTDIR)\Xlist.obj"
	-@erase "$(INTDIR)\Xlist.sbr"
	-@erase "$(INTDIR)\XListBox.obj"
	-@erase "$(INTDIR)\XListBox.sbr"
	-@erase "$(INTDIR)\XLodTerrain.obj"
	-@erase "$(INTDIR)\XLodTerrain.sbr"
	-@erase "$(INTDIR)\XMeshContainer.obj"
	-@erase "$(INTDIR)\XMeshContainer.sbr"
	-@erase "$(INTDIR)\XMeshMath.obj"
	-@erase "$(INTDIR)\XMeshMath.sbr"
	-@erase "$(INTDIR)\XMessageBox.obj"
	-@erase "$(INTDIR)\XMessageBox.sbr"
	-@erase "$(INTDIR)\XMob.obj"
	-@erase "$(INTDIR)\XMob.sbr"
	-@erase "$(INTDIR)\XModelContainer.obj"
	-@erase "$(INTDIR)\XModelContainer.sbr"
	-@erase "$(INTDIR)\XMString.obj"
	-@erase "$(INTDIR)\XMString.sbr"
	-@erase "$(INTDIR)\XNetwork.obj"
	-@erase "$(INTDIR)\XNetwork.sbr"
	-@erase "$(INTDIR)\XNPC.obj"
	-@erase "$(INTDIR)\XNPC.sbr"
	-@erase "$(INTDIR)\XNPCScript.obj"
	-@erase "$(INTDIR)\XNPCScript.sbr"
	-@erase "$(INTDIR)\XObjectManager.obj"
	-@erase "$(INTDIR)\XObjectManager.sbr"
	-@erase "$(INTDIR)\XPackage.obj"
	-@erase "$(INTDIR)\XPackage.sbr"
	-@erase "$(INTDIR)\XPathFinder.obj"
	-@erase "$(INTDIR)\XPathFinder.sbr"
	-@erase "$(INTDIR)\XProc_CreateCharacter.obj"
	-@erase "$(INTDIR)\XProc_CreateCharacter.sbr"
	-@erase "$(INTDIR)\XProc_FirstLoad.obj"
	-@erase "$(INTDIR)\XProc_FirstLoad.sbr"
	-@erase "$(INTDIR)\XProc_Loading.obj"
	-@erase "$(INTDIR)\XProc_Loading.sbr"
	-@erase "$(INTDIR)\XProc_LoginServer.obj"
	-@erase "$(INTDIR)\XProc_LoginServer.sbr"
	-@erase "$(INTDIR)\XProc_MainGame.obj"
	-@erase "$(INTDIR)\XProc_MainGame.sbr"
	-@erase "$(INTDIR)\XProc_SelectCharacter.obj"
	-@erase "$(INTDIR)\XProc_SelectCharacter.sbr"
	-@erase "$(INTDIR)\XProcess.obj"
	-@erase "$(INTDIR)\XProcess.sbr"
	-@erase "$(INTDIR)\XQuestScriptItem.obj"
	-@erase "$(INTDIR)\XQuestScriptItem.sbr"
	-@erase "$(INTDIR)\XRegistry.obj"
	-@erase "$(INTDIR)\XRegistry.sbr"
	-@erase "$(INTDIR)\XRenderStateControl.obj"
	-@erase "$(INTDIR)\XRenderStateControl.sbr"
	-@erase "$(INTDIR)\XScrollBar.obj"
	-@erase "$(INTDIR)\XScrollBar.sbr"
	-@erase "$(INTDIR)\XSimpleEdit.obj"
	-@erase "$(INTDIR)\XSimpleEdit.sbr"
	-@erase "$(INTDIR)\XSoundKernel.obj"
	-@erase "$(INTDIR)\XSoundKernel.sbr"
	-@erase "$(INTDIR)\XStatic.obj"
	-@erase "$(INTDIR)\XStatic.sbr"
	-@erase "$(INTDIR)\XStringItem.obj"
	-@erase "$(INTDIR)\XStringItem.sbr"
	-@erase "$(INTDIR)\XTextureManager.obj"
	-@erase "$(INTDIR)\XTextureManager.sbr"
	-@erase "$(INTDIR)\XTimeCounter.obj"
	-@erase "$(INTDIR)\XTimeCounter.sbr"
	-@erase "$(INTDIR)\XTimer.obj"
	-@erase "$(INTDIR)\XTimer.sbr"
	-@erase "$(INTDIR)\XToolTip.obj"
	-@erase "$(INTDIR)\XToolTip.sbr"
	-@erase "$(INTDIR)\XUser.obj"
	-@erase "$(INTDIR)\XUser.sbr"
	-@erase "$(INTDIR)\XVSlider.obj"
	-@erase "$(INTDIR)\XVSlider.sbr"
	-@erase "$(INTDIR)\XWindow.obj"
	-@erase "$(INTDIR)\XWindow.sbr"
	-@erase "$(INTDIR)\XWindow_ChattingDefault.obj"
	-@erase "$(INTDIR)\XWindow_ChattingDefault.sbr"
	-@erase "$(INTDIR)\XWindow_ChattingMinimize.obj"
	-@erase "$(INTDIR)\XWindow_ChattingMinimize.sbr"
	-@erase "$(INTDIR)\XWindow_CInfo.obj"
	-@erase "$(INTDIR)\XWindow_CInfo.sbr"
	-@erase "$(INTDIR)\XWindow_FSWindow.obj"
	-@erase "$(INTDIR)\XWindow_FSWindow.sbr"
	-@erase "$(INTDIR)\XWindow_Inventory.obj"
	-@erase "$(INTDIR)\XWindow_Inventory.sbr"
	-@erase "$(INTDIR)\XWindow_ListBase.obj"
	-@erase "$(INTDIR)\XWindow_ListBase.sbr"
	-@erase "$(INTDIR)\XWindow_ModelDesc.obj"
	-@erase "$(INTDIR)\XWindow_ModelDesc.sbr"
	-@erase "$(INTDIR)\XWindow_ModelView.obj"
	-@erase "$(INTDIR)\XWindow_ModelView.sbr"
	-@erase "$(INTDIR)\XWindow_NPCScript.obj"
	-@erase "$(INTDIR)\XWindow_NPCScript.sbr"
	-@erase "$(INTDIR)\XWindow_Option.obj"
	-@erase "$(INTDIR)\XWindow_Option.sbr"
	-@erase "$(INTDIR)\XWindow_Party.obj"
	-@erase "$(INTDIR)\XWindow_Party.sbr"
	-@erase "$(INTDIR)\XWindow_PCTrade.obj"
	-@erase "$(INTDIR)\XWindow_PCTrade.sbr"
	-@erase "$(INTDIR)\XWindow_QSlot.obj"
	-@erase "$(INTDIR)\XWindow_QSlot.sbr"
	-@erase "$(INTDIR)\XWindow_QuestWindow.obj"
	-@erase "$(INTDIR)\XWindow_QuestWindow.sbr"
	-@erase "$(INTDIR)\XWindow_Skill.obj"
	-@erase "$(INTDIR)\XWindow_Skill.sbr"
	-@erase "$(INTDIR)\XWindow_SkillTooltip.obj"
	-@erase "$(INTDIR)\XWindow_SkillTooltip.sbr"
	-@erase "$(INTDIR)\XWindow_WorldMinimap.obj"
	-@erase "$(INTDIR)\XWindow_WorldMinimap.sbr"
	-@erase "$(INTDIR)\XWindowManager.obj"
	-@erase "$(INTDIR)\XWindowManager.sbr"
	-@erase "$(OUTDIR)\EmperorOfDragons.bsc"
	-@erase ".\Game\EmperorOfDragons.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./ErrorHandler" /I "./XWControl" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Particle" /I "./Script" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\EmperorOfDragons.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\EmperorOfDragons.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\EmperorOfDragons.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\EmperorOfDragons.sbr" \
	"$(INTDIR)\XDprintf.sbr" \
	"$(INTDIR)\XProcess.sbr" \
	"$(INTDIR)\XProc_LoginServer.sbr" \
	"$(INTDIR)\XProc_CreateCharacter.sbr" \
	"$(INTDIR)\XProc_SelectCharacter.sbr" \
	"$(INTDIR)\XWindow_ChattingDefault.sbr" \
	"$(INTDIR)\XWindow_ChattingMinimize.sbr" \
	"$(INTDIR)\XWindow_CInfo.sbr" \
	"$(INTDIR)\XWindow_FSWindow.sbr" \
	"$(INTDIR)\XWindow_Inventory.sbr" \
	"$(INTDIR)\XWindow_ListBase.sbr" \
	"$(INTDIR)\XWindow_ModelDesc.sbr" \
	"$(INTDIR)\XWindow_ModelView.sbr" \
	"$(INTDIR)\XWindow_NPCScript.sbr" \
	"$(INTDIR)\XWindow_Option.sbr" \
	"$(INTDIR)\XWindow_Party.sbr" \
	"$(INTDIR)\XWindow_PCTrade.sbr" \
	"$(INTDIR)\XWindow_QSlot.sbr" \
	"$(INTDIR)\XWindow_QuestWindow.sbr" \
	"$(INTDIR)\XWindow_Skill.sbr" \
	"$(INTDIR)\XWindow_SkillTooltip.sbr" \
	"$(INTDIR)\XWindow_WorldMinimap.sbr" \
	"$(INTDIR)\XProc_Loading.sbr" \
	"$(INTDIR)\XProc_MainGame.sbr" \
	"$(INTDIR)\XProc_FirstLoad.sbr" \
	"$(INTDIR)\GenericWindowClass.sbr" \
	"$(INTDIR)\MyBitmap.sbr" \
	"$(INTDIR)\XChatEdit.sbr" \
	"$(INTDIR)\XSimpleEdit.sbr" \
	"$(INTDIR)\Xitem.sbr" \
	"$(INTDIR)\Xlist.sbr" \
	"$(INTDIR)\XMob.sbr" \
	"$(INTDIR)\XNPC.sbr" \
	"$(INTDIR)\XObjectManager.sbr" \
	"$(INTDIR)\XUser.sbr" \
	"$(INTDIR)\CsuX3D_Box.sbr" \
	"$(INTDIR)\sucollide.sbr" \
	"$(INTDIR)\sumath.sbr" \
	"$(INTDIR)\d3dfont.sbr" \
	"$(INTDIR)\d3dutil.sbr" \
	"$(INTDIR)\dsutil.sbr" \
	"$(INTDIR)\dxutil.sbr" \
	"$(INTDIR)\XCompress.sbr" \
	"$(INTDIR)\XFastCode.sbr" \
	"$(INTDIR)\XPackage.sbr" \
	"$(INTDIR)\XRegistry.sbr" \
	"$(INTDIR)\XStringItem.sbr" \
	"$(INTDIR)\XTimeCounter.sbr" \
	"$(INTDIR)\XTimer.sbr" \
	"$(INTDIR)\XNetwork.sbr" \
	"$(INTDIR)\XBillboard.sbr" \
	"$(INTDIR)\XLensFlare.sbr" \
	"$(INTDIR)\XBipedController.sbr" \
	"$(INTDIR)\XMeshContainer.sbr" \
	"$(INTDIR)\KUWater.sbr" \
	"$(INTDIR)\X3DObject.sbr" \
	"$(INTDIR)\X3DObjectManager.sbr" \
	"$(INTDIR)\X3DPickedPrimitive.sbr" \
	"$(INTDIR)\X3PCamera.sbr" \
	"$(INTDIR)\XAnimationController.sbr" \
	"$(INTDIR)\XLodTerrain.sbr" \
	"$(INTDIR)\XMeshMath.sbr" \
	"$(INTDIR)\XModelContainer.sbr" \
	"$(INTDIR)\XRenderStateControl.sbr" \
	"$(INTDIR)\XTextureManager.sbr" \
	"$(INTDIR)\CObs.sbr" \
	"$(INTDIR)\XPathFinder.sbr" \
	"$(INTDIR)\MixerBase.sbr" \
	"$(INTDIR)\MixerFader.sbr" \
	"$(INTDIR)\SuMixerVolume.sbr" \
	"$(INTDIR)\SuMP3Play.sbr" \
	"$(INTDIR)\XSoundKernel.sbr" \
	"$(INTDIR)\X3DUFont.sbr" \
	"$(INTDIR)\XBallGauge.sbr" \
	"$(INTDIR)\XButton.sbr" \
	"$(INTDIR)\XCheckButton.sbr" \
	"$(INTDIR)\XGUIControlCenter.sbr" \
	"$(INTDIR)\XGUIObject.sbr" \
	"$(INTDIR)\XHSlider.sbr" \
	"$(INTDIR)\XImageStatic.sbr" \
	"$(INTDIR)\XListBox.sbr" \
	"$(INTDIR)\XMessageBox.sbr" \
	"$(INTDIR)\XMString.sbr" \
	"$(INTDIR)\XScrollBar.sbr" \
	"$(INTDIR)\XStatic.sbr" \
	"$(INTDIR)\XToolTip.sbr" \
	"$(INTDIR)\XVSlider.sbr" \
	"$(INTDIR)\XWindow.sbr" \
	"$(INTDIR)\XWindowManager.sbr" \
	"$(INTDIR)\C3TimesEffect.sbr" \
	"$(INTDIR)\CArmsSignal.sbr" \
	"$(INTDIR)\CEffectBillboard.sbr" \
	"$(INTDIR)\CEffectBlood.sbr" \
	"$(INTDIR)\CEffectMgr.sbr" \
	"$(INTDIR)\CEffectShadow.sbr" \
	"$(INTDIR)\CFootHold.sbr" \
	"$(INTDIR)\CircleParticle.sbr" \
	"$(INTDIR)\CirclePattern.sbr" \
	"$(INTDIR)\CLevUpEffect.sbr" \
	"$(INTDIR)\CLevUpTypeB.sbr" \
	"$(INTDIR)\CommonFuncs.sbr" \
	"$(INTDIR)\ControlFunUtil.sbr" \
	"$(INTDIR)\ParticleBillbord.sbr" \
	"$(INTDIR)\ParticleEmitter.sbr" \
	"$(INTDIR)\ParticleMgr.sbr" \
	"$(INTDIR)\Particlesys.sbr" \
	"$(INTDIR)\XNPCScript.sbr" \
	"$(INTDIR)\XQuestScriptItem.sbr"

"$(OUTDIR)\EmperorOfDragons.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=./Library/XGamebase.lib ./Library/XKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib d3dx8.lib dinput8.lib DxErr8.lib dsound.lib dxguid.lib ws2_32.lib imagehlp.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\EmperorOfDragons.pdb" /machine:I386 /out:"./Game/EmperorOfDragons.exe" 
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\EmperorOfDragons.obj" \
	"$(INTDIR)\XDprintf.obj" \
	"$(INTDIR)\XProcess.obj" \
	"$(INTDIR)\XProc_LoginServer.obj" \
	"$(INTDIR)\XProc_CreateCharacter.obj" \
	"$(INTDIR)\XProc_SelectCharacter.obj" \
	"$(INTDIR)\XWindow_ChattingDefault.obj" \
	"$(INTDIR)\XWindow_ChattingMinimize.obj" \
	"$(INTDIR)\XWindow_CInfo.obj" \
	"$(INTDIR)\XWindow_FSWindow.obj" \
	"$(INTDIR)\XWindow_Inventory.obj" \
	"$(INTDIR)\XWindow_ListBase.obj" \
	"$(INTDIR)\XWindow_ModelDesc.obj" \
	"$(INTDIR)\XWindow_ModelView.obj" \
	"$(INTDIR)\XWindow_NPCScript.obj" \
	"$(INTDIR)\XWindow_Option.obj" \
	"$(INTDIR)\XWindow_Party.obj" \
	"$(INTDIR)\XWindow_PCTrade.obj" \
	"$(INTDIR)\XWindow_QSlot.obj" \
	"$(INTDIR)\XWindow_QuestWindow.obj" \
	"$(INTDIR)\XWindow_Skill.obj" \
	"$(INTDIR)\XWindow_SkillTooltip.obj" \
	"$(INTDIR)\XWindow_WorldMinimap.obj" \
	"$(INTDIR)\XProc_Loading.obj" \
	"$(INTDIR)\XProc_MainGame.obj" \
	"$(INTDIR)\XProc_FirstLoad.obj" \
	"$(INTDIR)\GenericWindowClass.obj" \
	"$(INTDIR)\MyBitmap.obj" \
	"$(INTDIR)\XChatEdit.obj" \
	"$(INTDIR)\XSimpleEdit.obj" \
	"$(INTDIR)\Xitem.obj" \
	"$(INTDIR)\Xlist.obj" \
	"$(INTDIR)\XMob.obj" \
	"$(INTDIR)\XNPC.obj" \
	"$(INTDIR)\XObjectManager.obj" \
	"$(INTDIR)\XUser.obj" \
	"$(INTDIR)\CsuX3D_Box.obj" \
	"$(INTDIR)\sucollide.obj" \
	"$(INTDIR)\sumath.obj" \
	"$(INTDIR)\d3dfont.obj" \
	"$(INTDIR)\d3dutil.obj" \
	"$(INTDIR)\dsutil.obj" \
	"$(INTDIR)\dxutil.obj" \
	"$(INTDIR)\XCompress.obj" \
	"$(INTDIR)\XFastCode.obj" \
	"$(INTDIR)\XPackage.obj" \
	"$(INTDIR)\XRegistry.obj" \
	"$(INTDIR)\XStringItem.obj" \
	"$(INTDIR)\XTimeCounter.obj" \
	"$(INTDIR)\XTimer.obj" \
	"$(INTDIR)\XNetwork.obj" \
	"$(INTDIR)\XBillboard.obj" \
	"$(INTDIR)\XLensFlare.obj" \
	"$(INTDIR)\XBipedController.obj" \
	"$(INTDIR)\XMeshContainer.obj" \
	"$(INTDIR)\KUWater.obj" \
	"$(INTDIR)\X3DObject.obj" \
	"$(INTDIR)\X3DObjectManager.obj" \
	"$(INTDIR)\X3DPickedPrimitive.obj" \
	"$(INTDIR)\X3PCamera.obj" \
	"$(INTDIR)\XAnimationController.obj" \
	"$(INTDIR)\XLodTerrain.obj" \
	"$(INTDIR)\XMeshMath.obj" \
	"$(INTDIR)\XModelContainer.obj" \
	"$(INTDIR)\XRenderStateControl.obj" \
	"$(INTDIR)\XTextureManager.obj" \
	"$(INTDIR)\CObs.obj" \
	"$(INTDIR)\XPathFinder.obj" \
	"$(INTDIR)\MixerBase.obj" \
	"$(INTDIR)\MixerFader.obj" \
	"$(INTDIR)\SuMixerVolume.obj" \
	"$(INTDIR)\SuMP3Play.obj" \
	"$(INTDIR)\XSoundKernel.obj" \
	"$(INTDIR)\X3DUFont.obj" \
	"$(INTDIR)\XBallGauge.obj" \
	"$(INTDIR)\XButton.obj" \
	"$(INTDIR)\XCheckButton.obj" \
	"$(INTDIR)\XGUIControlCenter.obj" \
	"$(INTDIR)\XGUIObject.obj" \
	"$(INTDIR)\XHSlider.obj" \
	"$(INTDIR)\XImageStatic.obj" \
	"$(INTDIR)\XListBox.obj" \
	"$(INTDIR)\XMessageBox.obj" \
	"$(INTDIR)\XMString.obj" \
	"$(INTDIR)\XScrollBar.obj" \
	"$(INTDIR)\XStatic.obj" \
	"$(INTDIR)\XToolTip.obj" \
	"$(INTDIR)\XVSlider.obj" \
	"$(INTDIR)\XWindow.obj" \
	"$(INTDIR)\XWindowManager.obj" \
	"$(INTDIR)\C3TimesEffect.obj" \
	"$(INTDIR)\CArmsSignal.obj" \
	"$(INTDIR)\CEffectBillboard.obj" \
	"$(INTDIR)\CEffectBlood.obj" \
	"$(INTDIR)\CEffectMgr.obj" \
	"$(INTDIR)\CEffectShadow.obj" \
	"$(INTDIR)\CFootHold.obj" \
	"$(INTDIR)\CircleParticle.obj" \
	"$(INTDIR)\CirclePattern.obj" \
	"$(INTDIR)\CLevUpEffect.obj" \
	"$(INTDIR)\CLevUpTypeB.obj" \
	"$(INTDIR)\CommonFuncs.obj" \
	"$(INTDIR)\ControlFunUtil.obj" \
	"$(INTDIR)\ParticleBillbord.obj" \
	"$(INTDIR)\ParticleEmitter.obj" \
	"$(INTDIR)\ParticleMgr.obj" \
	"$(INTDIR)\Particlesys.obj" \
	"$(INTDIR)\XNPCScript.obj" \
	"$(INTDIR)\XQuestScriptItem.obj" \
	"$(INTDIR)\EmperorOfDragons.res" \
	".\Library\XGamebase.lib" \
	".\Library\XKernel.lib"

".\Game\EmperorOfDragons.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Compiled/release/EOD
# End Custom Macros

$(DS_POSTBUILD_DEP) : "XKernel - Win32 Release" "XGamebase - Win32 Release" ".\Game\EmperorOfDragons.exe" "$(OUTDIR)\EmperorOfDragons.bsc"
   rem copy .\Game\EmperorOfDragons.exe z:
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 Debug"

OUTDIR=.\Compiled/debug/EOD
INTDIR=.\Compiled/debug/EOD
# Begin Custom Macros
OutDir=.\Compiled/debug/EOD
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\Game\EmperorOfDragons_D.exe" "$(OUTDIR)\EmperorOfDragons.bsc"

!ELSE 

ALL : "XKernel - Win32 Debug" "XGamebase - Win32 Debug" ".\Game\EmperorOfDragons_D.exe" "$(OUTDIR)\EmperorOfDragons.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"XGamebase - Win32 DebugCLEAN" "XKernel - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\C3TimesEffect.obj"
	-@erase "$(INTDIR)\C3TimesEffect.sbr"
	-@erase "$(INTDIR)\CArmsSignal.obj"
	-@erase "$(INTDIR)\CArmsSignal.sbr"
	-@erase "$(INTDIR)\CEffectBillboard.obj"
	-@erase "$(INTDIR)\CEffectBillboard.sbr"
	-@erase "$(INTDIR)\CEffectBlood.obj"
	-@erase "$(INTDIR)\CEffectBlood.sbr"
	-@erase "$(INTDIR)\CEffectMgr.obj"
	-@erase "$(INTDIR)\CEffectMgr.sbr"
	-@erase "$(INTDIR)\CEffectShadow.obj"
	-@erase "$(INTDIR)\CEffectShadow.sbr"
	-@erase "$(INTDIR)\CFootHold.obj"
	-@erase "$(INTDIR)\CFootHold.sbr"
	-@erase "$(INTDIR)\CircleParticle.obj"
	-@erase "$(INTDIR)\CircleParticle.sbr"
	-@erase "$(INTDIR)\CirclePattern.obj"
	-@erase "$(INTDIR)\CirclePattern.sbr"
	-@erase "$(INTDIR)\CLevUpEffect.obj"
	-@erase "$(INTDIR)\CLevUpEffect.sbr"
	-@erase "$(INTDIR)\CLevUpTypeB.obj"
	-@erase "$(INTDIR)\CLevUpTypeB.sbr"
	-@erase "$(INTDIR)\CObs.obj"
	-@erase "$(INTDIR)\CObs.sbr"
	-@erase "$(INTDIR)\CommonFuncs.obj"
	-@erase "$(INTDIR)\CommonFuncs.sbr"
	-@erase "$(INTDIR)\ControlFunUtil.obj"
	-@erase "$(INTDIR)\ControlFunUtil.sbr"
	-@erase "$(INTDIR)\CsuX3D_Box.obj"
	-@erase "$(INTDIR)\CsuX3D_Box.sbr"
	-@erase "$(INTDIR)\d3dfont.obj"
	-@erase "$(INTDIR)\d3dfont.sbr"
	-@erase "$(INTDIR)\d3dutil.obj"
	-@erase "$(INTDIR)\d3dutil.sbr"
	-@erase "$(INTDIR)\dsutil.obj"
	-@erase "$(INTDIR)\dsutil.sbr"
	-@erase "$(INTDIR)\dxutil.obj"
	-@erase "$(INTDIR)\dxutil.sbr"
	-@erase "$(INTDIR)\EmperorOfDragons.obj"
	-@erase "$(INTDIR)\EmperorOfDragons.pch"
	-@erase "$(INTDIR)\EmperorOfDragons.res"
	-@erase "$(INTDIR)\EmperorOfDragons.sbr"
	-@erase "$(INTDIR)\GenericWindowClass.obj"
	-@erase "$(INTDIR)\GenericWindowClass.sbr"
	-@erase "$(INTDIR)\KUWater.obj"
	-@erase "$(INTDIR)\KUWater.sbr"
	-@erase "$(INTDIR)\MixerBase.obj"
	-@erase "$(INTDIR)\MixerBase.sbr"
	-@erase "$(INTDIR)\MixerFader.obj"
	-@erase "$(INTDIR)\MixerFader.sbr"
	-@erase "$(INTDIR)\MyBitmap.obj"
	-@erase "$(INTDIR)\MyBitmap.sbr"
	-@erase "$(INTDIR)\ParticleBillbord.obj"
	-@erase "$(INTDIR)\ParticleBillbord.sbr"
	-@erase "$(INTDIR)\ParticleEmitter.obj"
	-@erase "$(INTDIR)\ParticleEmitter.sbr"
	-@erase "$(INTDIR)\ParticleMgr.obj"
	-@erase "$(INTDIR)\ParticleMgr.sbr"
	-@erase "$(INTDIR)\Particlesys.obj"
	-@erase "$(INTDIR)\Particlesys.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\sucollide.obj"
	-@erase "$(INTDIR)\sucollide.sbr"
	-@erase "$(INTDIR)\sumath.obj"
	-@erase "$(INTDIR)\sumath.sbr"
	-@erase "$(INTDIR)\SuMixerVolume.obj"
	-@erase "$(INTDIR)\SuMixerVolume.sbr"
	-@erase "$(INTDIR)\SuMP3Play.obj"
	-@erase "$(INTDIR)\SuMP3Play.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\X3DObject.obj"
	-@erase "$(INTDIR)\X3DObject.sbr"
	-@erase "$(INTDIR)\X3DObjectManager.obj"
	-@erase "$(INTDIR)\X3DObjectManager.sbr"
	-@erase "$(INTDIR)\X3DPickedPrimitive.obj"
	-@erase "$(INTDIR)\X3DPickedPrimitive.sbr"
	-@erase "$(INTDIR)\X3DUFont.obj"
	-@erase "$(INTDIR)\X3DUFont.sbr"
	-@erase "$(INTDIR)\X3PCamera.obj"
	-@erase "$(INTDIR)\X3PCamera.sbr"
	-@erase "$(INTDIR)\XAnimationController.obj"
	-@erase "$(INTDIR)\XAnimationController.sbr"
	-@erase "$(INTDIR)\XBallGauge.obj"
	-@erase "$(INTDIR)\XBallGauge.sbr"
	-@erase "$(INTDIR)\XBillboard.obj"
	-@erase "$(INTDIR)\XBillboard.sbr"
	-@erase "$(INTDIR)\XBipedController.obj"
	-@erase "$(INTDIR)\XBipedController.sbr"
	-@erase "$(INTDIR)\XButton.obj"
	-@erase "$(INTDIR)\XButton.sbr"
	-@erase "$(INTDIR)\XChatEdit.obj"
	-@erase "$(INTDIR)\XChatEdit.sbr"
	-@erase "$(INTDIR)\XCheckButton.obj"
	-@erase "$(INTDIR)\XCheckButton.sbr"
	-@erase "$(INTDIR)\XCompress.obj"
	-@erase "$(INTDIR)\XCompress.sbr"
	-@erase "$(INTDIR)\XDprintf.obj"
	-@erase "$(INTDIR)\XDprintf.sbr"
	-@erase "$(INTDIR)\XFastCode.obj"
	-@erase "$(INTDIR)\XFastCode.sbr"
	-@erase "$(INTDIR)\XGUIControlCenter.obj"
	-@erase "$(INTDIR)\XGUIControlCenter.sbr"
	-@erase "$(INTDIR)\XGUIObject.obj"
	-@erase "$(INTDIR)\XGUIObject.sbr"
	-@erase "$(INTDIR)\XHSlider.obj"
	-@erase "$(INTDIR)\XHSlider.sbr"
	-@erase "$(INTDIR)\XImageStatic.obj"
	-@erase "$(INTDIR)\XImageStatic.sbr"
	-@erase "$(INTDIR)\Xitem.obj"
	-@erase "$(INTDIR)\Xitem.sbr"
	-@erase "$(INTDIR)\XLensFlare.obj"
	-@erase "$(INTDIR)\XLensFlare.sbr"
	-@erase "$(INTDIR)\Xlist.obj"
	-@erase "$(INTDIR)\Xlist.sbr"
	-@erase "$(INTDIR)\XListBox.obj"
	-@erase "$(INTDIR)\XListBox.sbr"
	-@erase "$(INTDIR)\XLodTerrain.obj"
	-@erase "$(INTDIR)\XLodTerrain.sbr"
	-@erase "$(INTDIR)\XMeshContainer.obj"
	-@erase "$(INTDIR)\XMeshContainer.sbr"
	-@erase "$(INTDIR)\XMeshMath.obj"
	-@erase "$(INTDIR)\XMeshMath.sbr"
	-@erase "$(INTDIR)\XMessageBox.obj"
	-@erase "$(INTDIR)\XMessageBox.sbr"
	-@erase "$(INTDIR)\XMob.obj"
	-@erase "$(INTDIR)\XMob.sbr"
	-@erase "$(INTDIR)\XModelContainer.obj"
	-@erase "$(INTDIR)\XModelContainer.sbr"
	-@erase "$(INTDIR)\XMString.obj"
	-@erase "$(INTDIR)\XMString.sbr"
	-@erase "$(INTDIR)\XNetwork.obj"
	-@erase "$(INTDIR)\XNetwork.sbr"
	-@erase "$(INTDIR)\XNPC.obj"
	-@erase "$(INTDIR)\XNPC.sbr"
	-@erase "$(INTDIR)\XNPCScript.obj"
	-@erase "$(INTDIR)\XNPCScript.sbr"
	-@erase "$(INTDIR)\XObjectManager.obj"
	-@erase "$(INTDIR)\XObjectManager.sbr"
	-@erase "$(INTDIR)\XPackage.obj"
	-@erase "$(INTDIR)\XPackage.sbr"
	-@erase "$(INTDIR)\XPathFinder.obj"
	-@erase "$(INTDIR)\XPathFinder.sbr"
	-@erase "$(INTDIR)\XProc_CreateCharacter.obj"
	-@erase "$(INTDIR)\XProc_CreateCharacter.sbr"
	-@erase "$(INTDIR)\XProc_FirstLoad.obj"
	-@erase "$(INTDIR)\XProc_FirstLoad.sbr"
	-@erase "$(INTDIR)\XProc_Loading.obj"
	-@erase "$(INTDIR)\XProc_Loading.sbr"
	-@erase "$(INTDIR)\XProc_LoginServer.obj"
	-@erase "$(INTDIR)\XProc_LoginServer.sbr"
	-@erase "$(INTDIR)\XProc_MainGame.obj"
	-@erase "$(INTDIR)\XProc_MainGame.sbr"
	-@erase "$(INTDIR)\XProc_SelectCharacter.obj"
	-@erase "$(INTDIR)\XProc_SelectCharacter.sbr"
	-@erase "$(INTDIR)\XProcess.obj"
	-@erase "$(INTDIR)\XProcess.sbr"
	-@erase "$(INTDIR)\XQuestScriptItem.obj"
	-@erase "$(INTDIR)\XQuestScriptItem.sbr"
	-@erase "$(INTDIR)\XRegistry.obj"
	-@erase "$(INTDIR)\XRegistry.sbr"
	-@erase "$(INTDIR)\XRenderStateControl.obj"
	-@erase "$(INTDIR)\XRenderStateControl.sbr"
	-@erase "$(INTDIR)\XScrollBar.obj"
	-@erase "$(INTDIR)\XScrollBar.sbr"
	-@erase "$(INTDIR)\XSimpleEdit.obj"
	-@erase "$(INTDIR)\XSimpleEdit.sbr"
	-@erase "$(INTDIR)\XSoundKernel.obj"
	-@erase "$(INTDIR)\XSoundKernel.sbr"
	-@erase "$(INTDIR)\XStatic.obj"
	-@erase "$(INTDIR)\XStatic.sbr"
	-@erase "$(INTDIR)\XStringItem.obj"
	-@erase "$(INTDIR)\XStringItem.sbr"
	-@erase "$(INTDIR)\XTextureManager.obj"
	-@erase "$(INTDIR)\XTextureManager.sbr"
	-@erase "$(INTDIR)\XTimeCounter.obj"
	-@erase "$(INTDIR)\XTimeCounter.sbr"
	-@erase "$(INTDIR)\XTimer.obj"
	-@erase "$(INTDIR)\XTimer.sbr"
	-@erase "$(INTDIR)\XToolTip.obj"
	-@erase "$(INTDIR)\XToolTip.sbr"
	-@erase "$(INTDIR)\XUser.obj"
	-@erase "$(INTDIR)\XUser.sbr"
	-@erase "$(INTDIR)\XVSlider.obj"
	-@erase "$(INTDIR)\XVSlider.sbr"
	-@erase "$(INTDIR)\XWindow.obj"
	-@erase "$(INTDIR)\XWindow.sbr"
	-@erase "$(INTDIR)\XWindow_ChattingDefault.obj"
	-@erase "$(INTDIR)\XWindow_ChattingDefault.sbr"
	-@erase "$(INTDIR)\XWindow_ChattingMinimize.obj"
	-@erase "$(INTDIR)\XWindow_ChattingMinimize.sbr"
	-@erase "$(INTDIR)\XWindow_CInfo.obj"
	-@erase "$(INTDIR)\XWindow_CInfo.sbr"
	-@erase "$(INTDIR)\XWindow_FSWindow.obj"
	-@erase "$(INTDIR)\XWindow_FSWindow.sbr"
	-@erase "$(INTDIR)\XWindow_Inventory.obj"
	-@erase "$(INTDIR)\XWindow_Inventory.sbr"
	-@erase "$(INTDIR)\XWindow_ListBase.obj"
	-@erase "$(INTDIR)\XWindow_ListBase.sbr"
	-@erase "$(INTDIR)\XWindow_ModelDesc.obj"
	-@erase "$(INTDIR)\XWindow_ModelDesc.sbr"
	-@erase "$(INTDIR)\XWindow_ModelView.obj"
	-@erase "$(INTDIR)\XWindow_ModelView.sbr"
	-@erase "$(INTDIR)\XWindow_NPCScript.obj"
	-@erase "$(INTDIR)\XWindow_NPCScript.sbr"
	-@erase "$(INTDIR)\XWindow_Option.obj"
	-@erase "$(INTDIR)\XWindow_Option.sbr"
	-@erase "$(INTDIR)\XWindow_Party.obj"
	-@erase "$(INTDIR)\XWindow_Party.sbr"
	-@erase "$(INTDIR)\XWindow_PCTrade.obj"
	-@erase "$(INTDIR)\XWindow_PCTrade.sbr"
	-@erase "$(INTDIR)\XWindow_QSlot.obj"
	-@erase "$(INTDIR)\XWindow_QSlot.sbr"
	-@erase "$(INTDIR)\XWindow_QuestWindow.obj"
	-@erase "$(INTDIR)\XWindow_QuestWindow.sbr"
	-@erase "$(INTDIR)\XWindow_Skill.obj"
	-@erase "$(INTDIR)\XWindow_Skill.sbr"
	-@erase "$(INTDIR)\XWindow_SkillTooltip.obj"
	-@erase "$(INTDIR)\XWindow_SkillTooltip.sbr"
	-@erase "$(INTDIR)\XWindow_WorldMinimap.obj"
	-@erase "$(INTDIR)\XWindow_WorldMinimap.sbr"
	-@erase "$(INTDIR)\XWindowManager.obj"
	-@erase "$(INTDIR)\XWindowManager.sbr"
	-@erase "$(OUTDIR)\EmperorOfDragons.bsc"
	-@erase "$(OUTDIR)\EmperorOfDragons_D.pdb"
	-@erase ".\Game\EmperorOfDragons_D.exe"
	-@erase ".\Game\EmperorOfDragons_D.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./ErrorHandler" /I "./XWControl" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Particle" /I "./Script" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\EmperorOfDragons.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\EmperorOfDragons.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\EmperorOfDragons.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\EmperorOfDragons.sbr" \
	"$(INTDIR)\XDprintf.sbr" \
	"$(INTDIR)\XProcess.sbr" \
	"$(INTDIR)\XProc_LoginServer.sbr" \
	"$(INTDIR)\XProc_CreateCharacter.sbr" \
	"$(INTDIR)\XProc_SelectCharacter.sbr" \
	"$(INTDIR)\XWindow_ChattingDefault.sbr" \
	"$(INTDIR)\XWindow_ChattingMinimize.sbr" \
	"$(INTDIR)\XWindow_CInfo.sbr" \
	"$(INTDIR)\XWindow_FSWindow.sbr" \
	"$(INTDIR)\XWindow_Inventory.sbr" \
	"$(INTDIR)\XWindow_ListBase.sbr" \
	"$(INTDIR)\XWindow_ModelDesc.sbr" \
	"$(INTDIR)\XWindow_ModelView.sbr" \
	"$(INTDIR)\XWindow_NPCScript.sbr" \
	"$(INTDIR)\XWindow_Option.sbr" \
	"$(INTDIR)\XWindow_Party.sbr" \
	"$(INTDIR)\XWindow_PCTrade.sbr" \
	"$(INTDIR)\XWindow_QSlot.sbr" \
	"$(INTDIR)\XWindow_QuestWindow.sbr" \
	"$(INTDIR)\XWindow_Skill.sbr" \
	"$(INTDIR)\XWindow_SkillTooltip.sbr" \
	"$(INTDIR)\XWindow_WorldMinimap.sbr" \
	"$(INTDIR)\XProc_Loading.sbr" \
	"$(INTDIR)\XProc_MainGame.sbr" \
	"$(INTDIR)\XProc_FirstLoad.sbr" \
	"$(INTDIR)\GenericWindowClass.sbr" \
	"$(INTDIR)\MyBitmap.sbr" \
	"$(INTDIR)\XChatEdit.sbr" \
	"$(INTDIR)\XSimpleEdit.sbr" \
	"$(INTDIR)\Xitem.sbr" \
	"$(INTDIR)\Xlist.sbr" \
	"$(INTDIR)\XMob.sbr" \
	"$(INTDIR)\XNPC.sbr" \
	"$(INTDIR)\XObjectManager.sbr" \
	"$(INTDIR)\XUser.sbr" \
	"$(INTDIR)\CsuX3D_Box.sbr" \
	"$(INTDIR)\sucollide.sbr" \
	"$(INTDIR)\sumath.sbr" \
	"$(INTDIR)\d3dfont.sbr" \
	"$(INTDIR)\d3dutil.sbr" \
	"$(INTDIR)\dsutil.sbr" \
	"$(INTDIR)\dxutil.sbr" \
	"$(INTDIR)\XCompress.sbr" \
	"$(INTDIR)\XFastCode.sbr" \
	"$(INTDIR)\XPackage.sbr" \
	"$(INTDIR)\XRegistry.sbr" \
	"$(INTDIR)\XStringItem.sbr" \
	"$(INTDIR)\XTimeCounter.sbr" \
	"$(INTDIR)\XTimer.sbr" \
	"$(INTDIR)\XNetwork.sbr" \
	"$(INTDIR)\XBillboard.sbr" \
	"$(INTDIR)\XLensFlare.sbr" \
	"$(INTDIR)\XBipedController.sbr" \
	"$(INTDIR)\XMeshContainer.sbr" \
	"$(INTDIR)\KUWater.sbr" \
	"$(INTDIR)\X3DObject.sbr" \
	"$(INTDIR)\X3DObjectManager.sbr" \
	"$(INTDIR)\X3DPickedPrimitive.sbr" \
	"$(INTDIR)\X3PCamera.sbr" \
	"$(INTDIR)\XAnimationController.sbr" \
	"$(INTDIR)\XLodTerrain.sbr" \
	"$(INTDIR)\XMeshMath.sbr" \
	"$(INTDIR)\XModelContainer.sbr" \
	"$(INTDIR)\XRenderStateControl.sbr" \
	"$(INTDIR)\XTextureManager.sbr" \
	"$(INTDIR)\CObs.sbr" \
	"$(INTDIR)\XPathFinder.sbr" \
	"$(INTDIR)\MixerBase.sbr" \
	"$(INTDIR)\MixerFader.sbr" \
	"$(INTDIR)\SuMixerVolume.sbr" \
	"$(INTDIR)\SuMP3Play.sbr" \
	"$(INTDIR)\XSoundKernel.sbr" \
	"$(INTDIR)\X3DUFont.sbr" \
	"$(INTDIR)\XBallGauge.sbr" \
	"$(INTDIR)\XButton.sbr" \
	"$(INTDIR)\XCheckButton.sbr" \
	"$(INTDIR)\XGUIControlCenter.sbr" \
	"$(INTDIR)\XGUIObject.sbr" \
	"$(INTDIR)\XHSlider.sbr" \
	"$(INTDIR)\XImageStatic.sbr" \
	"$(INTDIR)\XListBox.sbr" \
	"$(INTDIR)\XMessageBox.sbr" \
	"$(INTDIR)\XMString.sbr" \
	"$(INTDIR)\XScrollBar.sbr" \
	"$(INTDIR)\XStatic.sbr" \
	"$(INTDIR)\XToolTip.sbr" \
	"$(INTDIR)\XVSlider.sbr" \
	"$(INTDIR)\XWindow.sbr" \
	"$(INTDIR)\XWindowManager.sbr" \
	"$(INTDIR)\C3TimesEffect.sbr" \
	"$(INTDIR)\CArmsSignal.sbr" \
	"$(INTDIR)\CEffectBillboard.sbr" \
	"$(INTDIR)\CEffectBlood.sbr" \
	"$(INTDIR)\CEffectMgr.sbr" \
	"$(INTDIR)\CEffectShadow.sbr" \
	"$(INTDIR)\CFootHold.sbr" \
	"$(INTDIR)\CircleParticle.sbr" \
	"$(INTDIR)\CirclePattern.sbr" \
	"$(INTDIR)\CLevUpEffect.sbr" \
	"$(INTDIR)\CLevUpTypeB.sbr" \
	"$(INTDIR)\CommonFuncs.sbr" \
	"$(INTDIR)\ControlFunUtil.sbr" \
	"$(INTDIR)\ParticleBillbord.sbr" \
	"$(INTDIR)\ParticleEmitter.sbr" \
	"$(INTDIR)\ParticleMgr.sbr" \
	"$(INTDIR)\Particlesys.sbr" \
	"$(INTDIR)\XNPCScript.sbr" \
	"$(INTDIR)\XQuestScriptItem.sbr"

"$(OUTDIR)\EmperorOfDragons.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=./Library/XGamebaseD.lib ./Library/XKernelD.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib d3dx8.lib dsound.lib dinput8.lib DxErr8.lib dxguid.lib ws2_32.lib imagehlp.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\EmperorOfDragons_D.pdb" /debug /machine:I386 /out:"./Game/EmperorOfDragons_D.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\EmperorOfDragons.obj" \
	"$(INTDIR)\XDprintf.obj" \
	"$(INTDIR)\XProcess.obj" \
	"$(INTDIR)\XProc_LoginServer.obj" \
	"$(INTDIR)\XProc_CreateCharacter.obj" \
	"$(INTDIR)\XProc_SelectCharacter.obj" \
	"$(INTDIR)\XWindow_ChattingDefault.obj" \
	"$(INTDIR)\XWindow_ChattingMinimize.obj" \
	"$(INTDIR)\XWindow_CInfo.obj" \
	"$(INTDIR)\XWindow_FSWindow.obj" \
	"$(INTDIR)\XWindow_Inventory.obj" \
	"$(INTDIR)\XWindow_ListBase.obj" \
	"$(INTDIR)\XWindow_ModelDesc.obj" \
	"$(INTDIR)\XWindow_ModelView.obj" \
	"$(INTDIR)\XWindow_NPCScript.obj" \
	"$(INTDIR)\XWindow_Option.obj" \
	"$(INTDIR)\XWindow_Party.obj" \
	"$(INTDIR)\XWindow_PCTrade.obj" \
	"$(INTDIR)\XWindow_QSlot.obj" \
	"$(INTDIR)\XWindow_QuestWindow.obj" \
	"$(INTDIR)\XWindow_Skill.obj" \
	"$(INTDIR)\XWindow_SkillTooltip.obj" \
	"$(INTDIR)\XWindow_WorldMinimap.obj" \
	"$(INTDIR)\XProc_Loading.obj" \
	"$(INTDIR)\XProc_MainGame.obj" \
	"$(INTDIR)\XProc_FirstLoad.obj" \
	"$(INTDIR)\GenericWindowClass.obj" \
	"$(INTDIR)\MyBitmap.obj" \
	"$(INTDIR)\XChatEdit.obj" \
	"$(INTDIR)\XSimpleEdit.obj" \
	"$(INTDIR)\Xitem.obj" \
	"$(INTDIR)\Xlist.obj" \
	"$(INTDIR)\XMob.obj" \
	"$(INTDIR)\XNPC.obj" \
	"$(INTDIR)\XObjectManager.obj" \
	"$(INTDIR)\XUser.obj" \
	"$(INTDIR)\CsuX3D_Box.obj" \
	"$(INTDIR)\sucollide.obj" \
	"$(INTDIR)\sumath.obj" \
	"$(INTDIR)\d3dfont.obj" \
	"$(INTDIR)\d3dutil.obj" \
	"$(INTDIR)\dsutil.obj" \
	"$(INTDIR)\dxutil.obj" \
	"$(INTDIR)\XCompress.obj" \
	"$(INTDIR)\XFastCode.obj" \
	"$(INTDIR)\XPackage.obj" \
	"$(INTDIR)\XRegistry.obj" \
	"$(INTDIR)\XStringItem.obj" \
	"$(INTDIR)\XTimeCounter.obj" \
	"$(INTDIR)\XTimer.obj" \
	"$(INTDIR)\XNetwork.obj" \
	"$(INTDIR)\XBillboard.obj" \
	"$(INTDIR)\XLensFlare.obj" \
	"$(INTDIR)\XBipedController.obj" \
	"$(INTDIR)\XMeshContainer.obj" \
	"$(INTDIR)\KUWater.obj" \
	"$(INTDIR)\X3DObject.obj" \
	"$(INTDIR)\X3DObjectManager.obj" \
	"$(INTDIR)\X3DPickedPrimitive.obj" \
	"$(INTDIR)\X3PCamera.obj" \
	"$(INTDIR)\XAnimationController.obj" \
	"$(INTDIR)\XLodTerrain.obj" \
	"$(INTDIR)\XMeshMath.obj" \
	"$(INTDIR)\XModelContainer.obj" \
	"$(INTDIR)\XRenderStateControl.obj" \
	"$(INTDIR)\XTextureManager.obj" \
	"$(INTDIR)\CObs.obj" \
	"$(INTDIR)\XPathFinder.obj" \
	"$(INTDIR)\MixerBase.obj" \
	"$(INTDIR)\MixerFader.obj" \
	"$(INTDIR)\SuMixerVolume.obj" \
	"$(INTDIR)\SuMP3Play.obj" \
	"$(INTDIR)\XSoundKernel.obj" \
	"$(INTDIR)\X3DUFont.obj" \
	"$(INTDIR)\XBallGauge.obj" \
	"$(INTDIR)\XButton.obj" \
	"$(INTDIR)\XCheckButton.obj" \
	"$(INTDIR)\XGUIControlCenter.obj" \
	"$(INTDIR)\XGUIObject.obj" \
	"$(INTDIR)\XHSlider.obj" \
	"$(INTDIR)\XImageStatic.obj" \
	"$(INTDIR)\XListBox.obj" \
	"$(INTDIR)\XMessageBox.obj" \
	"$(INTDIR)\XMString.obj" \
	"$(INTDIR)\XScrollBar.obj" \
	"$(INTDIR)\XStatic.obj" \
	"$(INTDIR)\XToolTip.obj" \
	"$(INTDIR)\XVSlider.obj" \
	"$(INTDIR)\XWindow.obj" \
	"$(INTDIR)\XWindowManager.obj" \
	"$(INTDIR)\C3TimesEffect.obj" \
	"$(INTDIR)\CArmsSignal.obj" \
	"$(INTDIR)\CEffectBillboard.obj" \
	"$(INTDIR)\CEffectBlood.obj" \
	"$(INTDIR)\CEffectMgr.obj" \
	"$(INTDIR)\CEffectShadow.obj" \
	"$(INTDIR)\CFootHold.obj" \
	"$(INTDIR)\CircleParticle.obj" \
	"$(INTDIR)\CirclePattern.obj" \
	"$(INTDIR)\CLevUpEffect.obj" \
	"$(INTDIR)\CLevUpTypeB.obj" \
	"$(INTDIR)\CommonFuncs.obj" \
	"$(INTDIR)\ControlFunUtil.obj" \
	"$(INTDIR)\ParticleBillbord.obj" \
	"$(INTDIR)\ParticleEmitter.obj" \
	"$(INTDIR)\ParticleMgr.obj" \
	"$(INTDIR)\Particlesys.obj" \
	"$(INTDIR)\XNPCScript.obj" \
	"$(INTDIR)\XQuestScriptItem.obj" \
	"$(INTDIR)\EmperorOfDragons.res" \
	".\Library\XGamebased.lib" \
	".\Library\XKerneld.lib"

".\Game\EmperorOfDragons_D.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 Release_include_debugging_information"

OUTDIR=.\Compiled/ReleaseDebug/EOD
INTDIR=.\Compiled/ReleaseDebug/EOD
# Begin Custom Macros
OutDir=.\Compiled/ReleaseDebug/EOD
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\Game\EmperorOfDragons_RD.exe" "$(OUTDIR)\EmperorOfDragons.bsc"

!ELSE 

ALL : "XKernel - Win32 Release_include_debugging_information" "XGamebase - Win32 Release_include_debugging_information" ".\Game\EmperorOfDragons_RD.exe" "$(OUTDIR)\EmperorOfDragons.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"XGamebase - Win32 Release_include_debugging_informationCLEAN" "XKernel - Win32 Release_include_debugging_informationCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\C3TimesEffect.obj"
	-@erase "$(INTDIR)\C3TimesEffect.sbr"
	-@erase "$(INTDIR)\CArmsSignal.obj"
	-@erase "$(INTDIR)\CArmsSignal.sbr"
	-@erase "$(INTDIR)\CEffectBillboard.obj"
	-@erase "$(INTDIR)\CEffectBillboard.sbr"
	-@erase "$(INTDIR)\CEffectBlood.obj"
	-@erase "$(INTDIR)\CEffectBlood.sbr"
	-@erase "$(INTDIR)\CEffectMgr.obj"
	-@erase "$(INTDIR)\CEffectMgr.sbr"
	-@erase "$(INTDIR)\CEffectShadow.obj"
	-@erase "$(INTDIR)\CEffectShadow.sbr"
	-@erase "$(INTDIR)\CFootHold.obj"
	-@erase "$(INTDIR)\CFootHold.sbr"
	-@erase "$(INTDIR)\CircleParticle.obj"
	-@erase "$(INTDIR)\CircleParticle.sbr"
	-@erase "$(INTDIR)\CirclePattern.obj"
	-@erase "$(INTDIR)\CirclePattern.sbr"
	-@erase "$(INTDIR)\CLevUpEffect.obj"
	-@erase "$(INTDIR)\CLevUpEffect.sbr"
	-@erase "$(INTDIR)\CLevUpTypeB.obj"
	-@erase "$(INTDIR)\CLevUpTypeB.sbr"
	-@erase "$(INTDIR)\CObs.obj"
	-@erase "$(INTDIR)\CObs.sbr"
	-@erase "$(INTDIR)\CommonFuncs.obj"
	-@erase "$(INTDIR)\CommonFuncs.sbr"
	-@erase "$(INTDIR)\ControlFunUtil.obj"
	-@erase "$(INTDIR)\ControlFunUtil.sbr"
	-@erase "$(INTDIR)\CsuX3D_Box.obj"
	-@erase "$(INTDIR)\CsuX3D_Box.sbr"
	-@erase "$(INTDIR)\d3dfont.obj"
	-@erase "$(INTDIR)\d3dfont.sbr"
	-@erase "$(INTDIR)\d3dutil.obj"
	-@erase "$(INTDIR)\d3dutil.sbr"
	-@erase "$(INTDIR)\dsutil.obj"
	-@erase "$(INTDIR)\dsutil.sbr"
	-@erase "$(INTDIR)\dxutil.obj"
	-@erase "$(INTDIR)\dxutil.sbr"
	-@erase "$(INTDIR)\EmperorOfDragons.obj"
	-@erase "$(INTDIR)\EmperorOfDragons.pch"
	-@erase "$(INTDIR)\EmperorOfDragons.res"
	-@erase "$(INTDIR)\EmperorOfDragons.sbr"
	-@erase "$(INTDIR)\GenericWindowClass.obj"
	-@erase "$(INTDIR)\GenericWindowClass.sbr"
	-@erase "$(INTDIR)\KUWater.obj"
	-@erase "$(INTDIR)\KUWater.sbr"
	-@erase "$(INTDIR)\MixerBase.obj"
	-@erase "$(INTDIR)\MixerBase.sbr"
	-@erase "$(INTDIR)\MixerFader.obj"
	-@erase "$(INTDIR)\MixerFader.sbr"
	-@erase "$(INTDIR)\MyBitmap.obj"
	-@erase "$(INTDIR)\MyBitmap.sbr"
	-@erase "$(INTDIR)\ParticleBillbord.obj"
	-@erase "$(INTDIR)\ParticleBillbord.sbr"
	-@erase "$(INTDIR)\ParticleEmitter.obj"
	-@erase "$(INTDIR)\ParticleEmitter.sbr"
	-@erase "$(INTDIR)\ParticleMgr.obj"
	-@erase "$(INTDIR)\ParticleMgr.sbr"
	-@erase "$(INTDIR)\Particlesys.obj"
	-@erase "$(INTDIR)\Particlesys.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\sucollide.obj"
	-@erase "$(INTDIR)\sucollide.sbr"
	-@erase "$(INTDIR)\sumath.obj"
	-@erase "$(INTDIR)\sumath.sbr"
	-@erase "$(INTDIR)\SuMixerVolume.obj"
	-@erase "$(INTDIR)\SuMixerVolume.sbr"
	-@erase "$(INTDIR)\SuMP3Play.obj"
	-@erase "$(INTDIR)\SuMP3Play.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\X3DObject.obj"
	-@erase "$(INTDIR)\X3DObject.sbr"
	-@erase "$(INTDIR)\X3DObjectManager.obj"
	-@erase "$(INTDIR)\X3DObjectManager.sbr"
	-@erase "$(INTDIR)\X3DPickedPrimitive.obj"
	-@erase "$(INTDIR)\X3DPickedPrimitive.sbr"
	-@erase "$(INTDIR)\X3DUFont.obj"
	-@erase "$(INTDIR)\X3DUFont.sbr"
	-@erase "$(INTDIR)\X3PCamera.obj"
	-@erase "$(INTDIR)\X3PCamera.sbr"
	-@erase "$(INTDIR)\XAnimationController.obj"
	-@erase "$(INTDIR)\XAnimationController.sbr"
	-@erase "$(INTDIR)\XBallGauge.obj"
	-@erase "$(INTDIR)\XBallGauge.sbr"
	-@erase "$(INTDIR)\XBillboard.obj"
	-@erase "$(INTDIR)\XBillboard.sbr"
	-@erase "$(INTDIR)\XBipedController.obj"
	-@erase "$(INTDIR)\XBipedController.sbr"
	-@erase "$(INTDIR)\XButton.obj"
	-@erase "$(INTDIR)\XButton.sbr"
	-@erase "$(INTDIR)\XChatEdit.obj"
	-@erase "$(INTDIR)\XChatEdit.sbr"
	-@erase "$(INTDIR)\XCheckButton.obj"
	-@erase "$(INTDIR)\XCheckButton.sbr"
	-@erase "$(INTDIR)\XCompress.obj"
	-@erase "$(INTDIR)\XCompress.sbr"
	-@erase "$(INTDIR)\XDprintf.obj"
	-@erase "$(INTDIR)\XDprintf.sbr"
	-@erase "$(INTDIR)\XFastCode.obj"
	-@erase "$(INTDIR)\XFastCode.sbr"
	-@erase "$(INTDIR)\XGUIControlCenter.obj"
	-@erase "$(INTDIR)\XGUIControlCenter.sbr"
	-@erase "$(INTDIR)\XGUIObject.obj"
	-@erase "$(INTDIR)\XGUIObject.sbr"
	-@erase "$(INTDIR)\XHSlider.obj"
	-@erase "$(INTDIR)\XHSlider.sbr"
	-@erase "$(INTDIR)\XImageStatic.obj"
	-@erase "$(INTDIR)\XImageStatic.sbr"
	-@erase "$(INTDIR)\Xitem.obj"
	-@erase "$(INTDIR)\Xitem.sbr"
	-@erase "$(INTDIR)\XLensFlare.obj"
	-@erase "$(INTDIR)\XLensFlare.sbr"
	-@erase "$(INTDIR)\Xlist.obj"
	-@erase "$(INTDIR)\Xlist.sbr"
	-@erase "$(INTDIR)\XListBox.obj"
	-@erase "$(INTDIR)\XListBox.sbr"
	-@erase "$(INTDIR)\XLodTerrain.obj"
	-@erase "$(INTDIR)\XLodTerrain.sbr"
	-@erase "$(INTDIR)\XMeshContainer.obj"
	-@erase "$(INTDIR)\XMeshContainer.sbr"
	-@erase "$(INTDIR)\XMeshMath.obj"
	-@erase "$(INTDIR)\XMeshMath.sbr"
	-@erase "$(INTDIR)\XMessageBox.obj"
	-@erase "$(INTDIR)\XMessageBox.sbr"
	-@erase "$(INTDIR)\XMob.obj"
	-@erase "$(INTDIR)\XMob.sbr"
	-@erase "$(INTDIR)\XModelContainer.obj"
	-@erase "$(INTDIR)\XModelContainer.sbr"
	-@erase "$(INTDIR)\XMString.obj"
	-@erase "$(INTDIR)\XMString.sbr"
	-@erase "$(INTDIR)\XNetwork.obj"
	-@erase "$(INTDIR)\XNetwork.sbr"
	-@erase "$(INTDIR)\XNPC.obj"
	-@erase "$(INTDIR)\XNPC.sbr"
	-@erase "$(INTDIR)\XNPCScript.obj"
	-@erase "$(INTDIR)\XNPCScript.sbr"
	-@erase "$(INTDIR)\XObjectManager.obj"
	-@erase "$(INTDIR)\XObjectManager.sbr"
	-@erase "$(INTDIR)\XPackage.obj"
	-@erase "$(INTDIR)\XPackage.sbr"
	-@erase "$(INTDIR)\XPathFinder.obj"
	-@erase "$(INTDIR)\XPathFinder.sbr"
	-@erase "$(INTDIR)\XProc_CreateCharacter.obj"
	-@erase "$(INTDIR)\XProc_CreateCharacter.sbr"
	-@erase "$(INTDIR)\XProc_FirstLoad.obj"
	-@erase "$(INTDIR)\XProc_FirstLoad.sbr"
	-@erase "$(INTDIR)\XProc_Loading.obj"
	-@erase "$(INTDIR)\XProc_Loading.sbr"
	-@erase "$(INTDIR)\XProc_LoginServer.obj"
	-@erase "$(INTDIR)\XProc_LoginServer.sbr"
	-@erase "$(INTDIR)\XProc_MainGame.obj"
	-@erase "$(INTDIR)\XProc_MainGame.sbr"
	-@erase "$(INTDIR)\XProc_SelectCharacter.obj"
	-@erase "$(INTDIR)\XProc_SelectCharacter.sbr"
	-@erase "$(INTDIR)\XProcess.obj"
	-@erase "$(INTDIR)\XProcess.sbr"
	-@erase "$(INTDIR)\XQuestScriptItem.obj"
	-@erase "$(INTDIR)\XQuestScriptItem.sbr"
	-@erase "$(INTDIR)\XRegistry.obj"
	-@erase "$(INTDIR)\XRegistry.sbr"
	-@erase "$(INTDIR)\XRenderStateControl.obj"
	-@erase "$(INTDIR)\XRenderStateControl.sbr"
	-@erase "$(INTDIR)\XScrollBar.obj"
	-@erase "$(INTDIR)\XScrollBar.sbr"
	-@erase "$(INTDIR)\XSimpleEdit.obj"
	-@erase "$(INTDIR)\XSimpleEdit.sbr"
	-@erase "$(INTDIR)\XSoundKernel.obj"
	-@erase "$(INTDIR)\XSoundKernel.sbr"
	-@erase "$(INTDIR)\XStatic.obj"
	-@erase "$(INTDIR)\XStatic.sbr"
	-@erase "$(INTDIR)\XStringItem.obj"
	-@erase "$(INTDIR)\XStringItem.sbr"
	-@erase "$(INTDIR)\XTextureManager.obj"
	-@erase "$(INTDIR)\XTextureManager.sbr"
	-@erase "$(INTDIR)\XTimeCounter.obj"
	-@erase "$(INTDIR)\XTimeCounter.sbr"
	-@erase "$(INTDIR)\XTimer.obj"
	-@erase "$(INTDIR)\XTimer.sbr"
	-@erase "$(INTDIR)\XToolTip.obj"
	-@erase "$(INTDIR)\XToolTip.sbr"
	-@erase "$(INTDIR)\XUser.obj"
	-@erase "$(INTDIR)\XUser.sbr"
	-@erase "$(INTDIR)\XVSlider.obj"
	-@erase "$(INTDIR)\XVSlider.sbr"
	-@erase "$(INTDIR)\XWindow.obj"
	-@erase "$(INTDIR)\XWindow.sbr"
	-@erase "$(INTDIR)\XWindow_ChattingDefault.obj"
	-@erase "$(INTDIR)\XWindow_ChattingDefault.sbr"
	-@erase "$(INTDIR)\XWindow_ChattingMinimize.obj"
	-@erase "$(INTDIR)\XWindow_ChattingMinimize.sbr"
	-@erase "$(INTDIR)\XWindow_CInfo.obj"
	-@erase "$(INTDIR)\XWindow_CInfo.sbr"
	-@erase "$(INTDIR)\XWindow_FSWindow.obj"
	-@erase "$(INTDIR)\XWindow_FSWindow.sbr"
	-@erase "$(INTDIR)\XWindow_Inventory.obj"
	-@erase "$(INTDIR)\XWindow_Inventory.sbr"
	-@erase "$(INTDIR)\XWindow_ListBase.obj"
	-@erase "$(INTDIR)\XWindow_ListBase.sbr"
	-@erase "$(INTDIR)\XWindow_ModelDesc.obj"
	-@erase "$(INTDIR)\XWindow_ModelDesc.sbr"
	-@erase "$(INTDIR)\XWindow_ModelView.obj"
	-@erase "$(INTDIR)\XWindow_ModelView.sbr"
	-@erase "$(INTDIR)\XWindow_NPCScript.obj"
	-@erase "$(INTDIR)\XWindow_NPCScript.sbr"
	-@erase "$(INTDIR)\XWindow_Option.obj"
	-@erase "$(INTDIR)\XWindow_Option.sbr"
	-@erase "$(INTDIR)\XWindow_Party.obj"
	-@erase "$(INTDIR)\XWindow_Party.sbr"
	-@erase "$(INTDIR)\XWindow_PCTrade.obj"
	-@erase "$(INTDIR)\XWindow_PCTrade.sbr"
	-@erase "$(INTDIR)\XWindow_QSlot.obj"
	-@erase "$(INTDIR)\XWindow_QSlot.sbr"
	-@erase "$(INTDIR)\XWindow_QuestWindow.obj"
	-@erase "$(INTDIR)\XWindow_QuestWindow.sbr"
	-@erase "$(INTDIR)\XWindow_Skill.obj"
	-@erase "$(INTDIR)\XWindow_Skill.sbr"
	-@erase "$(INTDIR)\XWindow_SkillTooltip.obj"
	-@erase "$(INTDIR)\XWindow_SkillTooltip.sbr"
	-@erase "$(INTDIR)\XWindow_WorldMinimap.obj"
	-@erase "$(INTDIR)\XWindow_WorldMinimap.sbr"
	-@erase "$(INTDIR)\XWindowManager.obj"
	-@erase "$(INTDIR)\XWindowManager.sbr"
	-@erase "$(OUTDIR)\EmperorOfDragons.bsc"
	-@erase "$(OUTDIR)\EmperorOfDragons_RD.pdb"
	-@erase ".\Game\EmperorOfDragons_RD.exe"
	-@erase ".\Game\EmperorOfDragons_RD.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./ErrorHandler" /I "./XWControl" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Particle" /I "./Script" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /FAs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\EmperorOfDragons.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\EmperorOfDragons.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\EmperorOfDragons.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\EmperorOfDragons.sbr" \
	"$(INTDIR)\XDprintf.sbr" \
	"$(INTDIR)\XProcess.sbr" \
	"$(INTDIR)\XProc_LoginServer.sbr" \
	"$(INTDIR)\XProc_CreateCharacter.sbr" \
	"$(INTDIR)\XProc_SelectCharacter.sbr" \
	"$(INTDIR)\XWindow_ChattingDefault.sbr" \
	"$(INTDIR)\XWindow_ChattingMinimize.sbr" \
	"$(INTDIR)\XWindow_CInfo.sbr" \
	"$(INTDIR)\XWindow_FSWindow.sbr" \
	"$(INTDIR)\XWindow_Inventory.sbr" \
	"$(INTDIR)\XWindow_ListBase.sbr" \
	"$(INTDIR)\XWindow_ModelDesc.sbr" \
	"$(INTDIR)\XWindow_ModelView.sbr" \
	"$(INTDIR)\XWindow_NPCScript.sbr" \
	"$(INTDIR)\XWindow_Option.sbr" \
	"$(INTDIR)\XWindow_Party.sbr" \
	"$(INTDIR)\XWindow_PCTrade.sbr" \
	"$(INTDIR)\XWindow_QSlot.sbr" \
	"$(INTDIR)\XWindow_QuestWindow.sbr" \
	"$(INTDIR)\XWindow_Skill.sbr" \
	"$(INTDIR)\XWindow_SkillTooltip.sbr" \
	"$(INTDIR)\XWindow_WorldMinimap.sbr" \
	"$(INTDIR)\XProc_Loading.sbr" \
	"$(INTDIR)\XProc_MainGame.sbr" \
	"$(INTDIR)\XProc_FirstLoad.sbr" \
	"$(INTDIR)\GenericWindowClass.sbr" \
	"$(INTDIR)\MyBitmap.sbr" \
	"$(INTDIR)\XChatEdit.sbr" \
	"$(INTDIR)\XSimpleEdit.sbr" \
	"$(INTDIR)\Xitem.sbr" \
	"$(INTDIR)\Xlist.sbr" \
	"$(INTDIR)\XMob.sbr" \
	"$(INTDIR)\XNPC.sbr" \
	"$(INTDIR)\XObjectManager.sbr" \
	"$(INTDIR)\XUser.sbr" \
	"$(INTDIR)\CsuX3D_Box.sbr" \
	"$(INTDIR)\sucollide.sbr" \
	"$(INTDIR)\sumath.sbr" \
	"$(INTDIR)\d3dfont.sbr" \
	"$(INTDIR)\d3dutil.sbr" \
	"$(INTDIR)\dsutil.sbr" \
	"$(INTDIR)\dxutil.sbr" \
	"$(INTDIR)\XCompress.sbr" \
	"$(INTDIR)\XFastCode.sbr" \
	"$(INTDIR)\XPackage.sbr" \
	"$(INTDIR)\XRegistry.sbr" \
	"$(INTDIR)\XStringItem.sbr" \
	"$(INTDIR)\XTimeCounter.sbr" \
	"$(INTDIR)\XTimer.sbr" \
	"$(INTDIR)\XNetwork.sbr" \
	"$(INTDIR)\XBillboard.sbr" \
	"$(INTDIR)\XLensFlare.sbr" \
	"$(INTDIR)\XBipedController.sbr" \
	"$(INTDIR)\XMeshContainer.sbr" \
	"$(INTDIR)\KUWater.sbr" \
	"$(INTDIR)\X3DObject.sbr" \
	"$(INTDIR)\X3DObjectManager.sbr" \
	"$(INTDIR)\X3DPickedPrimitive.sbr" \
	"$(INTDIR)\X3PCamera.sbr" \
	"$(INTDIR)\XAnimationController.sbr" \
	"$(INTDIR)\XLodTerrain.sbr" \
	"$(INTDIR)\XMeshMath.sbr" \
	"$(INTDIR)\XModelContainer.sbr" \
	"$(INTDIR)\XRenderStateControl.sbr" \
	"$(INTDIR)\XTextureManager.sbr" \
	"$(INTDIR)\CObs.sbr" \
	"$(INTDIR)\XPathFinder.sbr" \
	"$(INTDIR)\MixerBase.sbr" \
	"$(INTDIR)\MixerFader.sbr" \
	"$(INTDIR)\SuMixerVolume.sbr" \
	"$(INTDIR)\SuMP3Play.sbr" \
	"$(INTDIR)\XSoundKernel.sbr" \
	"$(INTDIR)\X3DUFont.sbr" \
	"$(INTDIR)\XBallGauge.sbr" \
	"$(INTDIR)\XButton.sbr" \
	"$(INTDIR)\XCheckButton.sbr" \
	"$(INTDIR)\XGUIControlCenter.sbr" \
	"$(INTDIR)\XGUIObject.sbr" \
	"$(INTDIR)\XHSlider.sbr" \
	"$(INTDIR)\XImageStatic.sbr" \
	"$(INTDIR)\XListBox.sbr" \
	"$(INTDIR)\XMessageBox.sbr" \
	"$(INTDIR)\XMString.sbr" \
	"$(INTDIR)\XScrollBar.sbr" \
	"$(INTDIR)\XStatic.sbr" \
	"$(INTDIR)\XToolTip.sbr" \
	"$(INTDIR)\XVSlider.sbr" \
	"$(INTDIR)\XWindow.sbr" \
	"$(INTDIR)\XWindowManager.sbr" \
	"$(INTDIR)\C3TimesEffect.sbr" \
	"$(INTDIR)\CArmsSignal.sbr" \
	"$(INTDIR)\CEffectBillboard.sbr" \
	"$(INTDIR)\CEffectBlood.sbr" \
	"$(INTDIR)\CEffectMgr.sbr" \
	"$(INTDIR)\CEffectShadow.sbr" \
	"$(INTDIR)\CFootHold.sbr" \
	"$(INTDIR)\CircleParticle.sbr" \
	"$(INTDIR)\CirclePattern.sbr" \
	"$(INTDIR)\CLevUpEffect.sbr" \
	"$(INTDIR)\CLevUpTypeB.sbr" \
	"$(INTDIR)\CommonFuncs.sbr" \
	"$(INTDIR)\ControlFunUtil.sbr" \
	"$(INTDIR)\ParticleBillbord.sbr" \
	"$(INTDIR)\ParticleEmitter.sbr" \
	"$(INTDIR)\ParticleMgr.sbr" \
	"$(INTDIR)\Particlesys.sbr" \
	"$(INTDIR)\XNPCScript.sbr" \
	"$(INTDIR)\XQuestScriptItem.sbr"

"$(OUTDIR)\EmperorOfDragons.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=./Library/XGamebase_RD.lib ./Library/XKernel_RD.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib d3dx8.lib dinput8.lib dsound.lib DxErr8.lib dxguid.lib ws2_32.lib imagehlp.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\EmperorOfDragons_RD.pdb" /debug /machine:I386 /out:"./Game/EmperorOfDragons_RD.exe" 
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\EmperorOfDragons.obj" \
	"$(INTDIR)\XDprintf.obj" \
	"$(INTDIR)\XProcess.obj" \
	"$(INTDIR)\XProc_LoginServer.obj" \
	"$(INTDIR)\XProc_CreateCharacter.obj" \
	"$(INTDIR)\XProc_SelectCharacter.obj" \
	"$(INTDIR)\XWindow_ChattingDefault.obj" \
	"$(INTDIR)\XWindow_ChattingMinimize.obj" \
	"$(INTDIR)\XWindow_CInfo.obj" \
	"$(INTDIR)\XWindow_FSWindow.obj" \
	"$(INTDIR)\XWindow_Inventory.obj" \
	"$(INTDIR)\XWindow_ListBase.obj" \
	"$(INTDIR)\XWindow_ModelDesc.obj" \
	"$(INTDIR)\XWindow_ModelView.obj" \
	"$(INTDIR)\XWindow_NPCScript.obj" \
	"$(INTDIR)\XWindow_Option.obj" \
	"$(INTDIR)\XWindow_Party.obj" \
	"$(INTDIR)\XWindow_PCTrade.obj" \
	"$(INTDIR)\XWindow_QSlot.obj" \
	"$(INTDIR)\XWindow_QuestWindow.obj" \
	"$(INTDIR)\XWindow_Skill.obj" \
	"$(INTDIR)\XWindow_SkillTooltip.obj" \
	"$(INTDIR)\XWindow_WorldMinimap.obj" \
	"$(INTDIR)\XProc_Loading.obj" \
	"$(INTDIR)\XProc_MainGame.obj" \
	"$(INTDIR)\XProc_FirstLoad.obj" \
	"$(INTDIR)\GenericWindowClass.obj" \
	"$(INTDIR)\MyBitmap.obj" \
	"$(INTDIR)\XChatEdit.obj" \
	"$(INTDIR)\XSimpleEdit.obj" \
	"$(INTDIR)\Xitem.obj" \
	"$(INTDIR)\Xlist.obj" \
	"$(INTDIR)\XMob.obj" \
	"$(INTDIR)\XNPC.obj" \
	"$(INTDIR)\XObjectManager.obj" \
	"$(INTDIR)\XUser.obj" \
	"$(INTDIR)\CsuX3D_Box.obj" \
	"$(INTDIR)\sucollide.obj" \
	"$(INTDIR)\sumath.obj" \
	"$(INTDIR)\d3dfont.obj" \
	"$(INTDIR)\d3dutil.obj" \
	"$(INTDIR)\dsutil.obj" \
	"$(INTDIR)\dxutil.obj" \
	"$(INTDIR)\XCompress.obj" \
	"$(INTDIR)\XFastCode.obj" \
	"$(INTDIR)\XPackage.obj" \
	"$(INTDIR)\XRegistry.obj" \
	"$(INTDIR)\XStringItem.obj" \
	"$(INTDIR)\XTimeCounter.obj" \
	"$(INTDIR)\XTimer.obj" \
	"$(INTDIR)\XNetwork.obj" \
	"$(INTDIR)\XBillboard.obj" \
	"$(INTDIR)\XLensFlare.obj" \
	"$(INTDIR)\XBipedController.obj" \
	"$(INTDIR)\XMeshContainer.obj" \
	"$(INTDIR)\KUWater.obj" \
	"$(INTDIR)\X3DObject.obj" \
	"$(INTDIR)\X3DObjectManager.obj" \
	"$(INTDIR)\X3DPickedPrimitive.obj" \
	"$(INTDIR)\X3PCamera.obj" \
	"$(INTDIR)\XAnimationController.obj" \
	"$(INTDIR)\XLodTerrain.obj" \
	"$(INTDIR)\XMeshMath.obj" \
	"$(INTDIR)\XModelContainer.obj" \
	"$(INTDIR)\XRenderStateControl.obj" \
	"$(INTDIR)\XTextureManager.obj" \
	"$(INTDIR)\CObs.obj" \
	"$(INTDIR)\XPathFinder.obj" \
	"$(INTDIR)\MixerBase.obj" \
	"$(INTDIR)\MixerFader.obj" \
	"$(INTDIR)\SuMixerVolume.obj" \
	"$(INTDIR)\SuMP3Play.obj" \
	"$(INTDIR)\XSoundKernel.obj" \
	"$(INTDIR)\X3DUFont.obj" \
	"$(INTDIR)\XBallGauge.obj" \
	"$(INTDIR)\XButton.obj" \
	"$(INTDIR)\XCheckButton.obj" \
	"$(INTDIR)\XGUIControlCenter.obj" \
	"$(INTDIR)\XGUIObject.obj" \
	"$(INTDIR)\XHSlider.obj" \
	"$(INTDIR)\XImageStatic.obj" \
	"$(INTDIR)\XListBox.obj" \
	"$(INTDIR)\XMessageBox.obj" \
	"$(INTDIR)\XMString.obj" \
	"$(INTDIR)\XScrollBar.obj" \
	"$(INTDIR)\XStatic.obj" \
	"$(INTDIR)\XToolTip.obj" \
	"$(INTDIR)\XVSlider.obj" \
	"$(INTDIR)\XWindow.obj" \
	"$(INTDIR)\XWindowManager.obj" \
	"$(INTDIR)\C3TimesEffect.obj" \
	"$(INTDIR)\CArmsSignal.obj" \
	"$(INTDIR)\CEffectBillboard.obj" \
	"$(INTDIR)\CEffectBlood.obj" \
	"$(INTDIR)\CEffectMgr.obj" \
	"$(INTDIR)\CEffectShadow.obj" \
	"$(INTDIR)\CFootHold.obj" \
	"$(INTDIR)\CircleParticle.obj" \
	"$(INTDIR)\CirclePattern.obj" \
	"$(INTDIR)\CLevUpEffect.obj" \
	"$(INTDIR)\CLevUpTypeB.obj" \
	"$(INTDIR)\CommonFuncs.obj" \
	"$(INTDIR)\ControlFunUtil.obj" \
	"$(INTDIR)\ParticleBillbord.obj" \
	"$(INTDIR)\ParticleEmitter.obj" \
	"$(INTDIR)\ParticleMgr.obj" \
	"$(INTDIR)\Particlesys.obj" \
	"$(INTDIR)\XNPCScript.obj" \
	"$(INTDIR)\XQuestScriptItem.obj" \
	"$(INTDIR)\EmperorOfDragons.res" \
	".\Library\XGamebase_RD.lib" \
	".\Library\XKernel_RD.lib"

".\Game\EmperorOfDragons_RD.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Compiled/ReleaseDebug/EOD
# End Custom Macros

$(DS_POSTBUILD_DEP) : "XKernel - Win32 Release_include_debugging_information" "XGamebase - Win32 Release_include_debugging_information" ".\Game\EmperorOfDragons_RD.exe" "$(OUTDIR)\EmperorOfDragons.bsc"
   rem copy .\Game\EmperorOfDragons_RD.exe z:
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 Release_Manager"

OUTDIR=.\Compiled/releasemanager/EOD
INTDIR=.\Compiled/releasemanager/EOD
# Begin Custom Macros
OutDir=.\Compiled/releasemanager/EOD
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\Game\EmperorOfDragons_M.exe" "$(OUTDIR)\EmperorOfDragons.bsc"

!ELSE 

ALL : "XKernel - Win32 Release_Manager" "XGamebase - Win32 Release_Manager" ".\Game\EmperorOfDragons_M.exe" "$(OUTDIR)\EmperorOfDragons.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"XGamebase - Win32 Release_ManagerCLEAN" "XKernel - Win32 Release_ManagerCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\C3TimesEffect.obj"
	-@erase "$(INTDIR)\C3TimesEffect.sbr"
	-@erase "$(INTDIR)\CArmsSignal.obj"
	-@erase "$(INTDIR)\CArmsSignal.sbr"
	-@erase "$(INTDIR)\CEffectBillboard.obj"
	-@erase "$(INTDIR)\CEffectBillboard.sbr"
	-@erase "$(INTDIR)\CEffectBlood.obj"
	-@erase "$(INTDIR)\CEffectBlood.sbr"
	-@erase "$(INTDIR)\CEffectMgr.obj"
	-@erase "$(INTDIR)\CEffectMgr.sbr"
	-@erase "$(INTDIR)\CEffectShadow.obj"
	-@erase "$(INTDIR)\CEffectShadow.sbr"
	-@erase "$(INTDIR)\CFootHold.obj"
	-@erase "$(INTDIR)\CFootHold.sbr"
	-@erase "$(INTDIR)\CircleParticle.obj"
	-@erase "$(INTDIR)\CircleParticle.sbr"
	-@erase "$(INTDIR)\CirclePattern.obj"
	-@erase "$(INTDIR)\CirclePattern.sbr"
	-@erase "$(INTDIR)\CLevUpEffect.obj"
	-@erase "$(INTDIR)\CLevUpEffect.sbr"
	-@erase "$(INTDIR)\CLevUpTypeB.obj"
	-@erase "$(INTDIR)\CLevUpTypeB.sbr"
	-@erase "$(INTDIR)\CObs.obj"
	-@erase "$(INTDIR)\CObs.sbr"
	-@erase "$(INTDIR)\CommonFuncs.obj"
	-@erase "$(INTDIR)\CommonFuncs.sbr"
	-@erase "$(INTDIR)\ControlFunUtil.obj"
	-@erase "$(INTDIR)\ControlFunUtil.sbr"
	-@erase "$(INTDIR)\CsuX3D_Box.obj"
	-@erase "$(INTDIR)\CsuX3D_Box.sbr"
	-@erase "$(INTDIR)\d3dfont.obj"
	-@erase "$(INTDIR)\d3dfont.sbr"
	-@erase "$(INTDIR)\d3dutil.obj"
	-@erase "$(INTDIR)\d3dutil.sbr"
	-@erase "$(INTDIR)\dsutil.obj"
	-@erase "$(INTDIR)\dsutil.sbr"
	-@erase "$(INTDIR)\dxutil.obj"
	-@erase "$(INTDIR)\dxutil.sbr"
	-@erase "$(INTDIR)\EmperorOfDragons.obj"
	-@erase "$(INTDIR)\EmperorOfDragons.pch"
	-@erase "$(INTDIR)\EmperorOfDragons.res"
	-@erase "$(INTDIR)\EmperorOfDragons.sbr"
	-@erase "$(INTDIR)\GenericWindowClass.obj"
	-@erase "$(INTDIR)\GenericWindowClass.sbr"
	-@erase "$(INTDIR)\KUWater.obj"
	-@erase "$(INTDIR)\KUWater.sbr"
	-@erase "$(INTDIR)\MixerBase.obj"
	-@erase "$(INTDIR)\MixerBase.sbr"
	-@erase "$(INTDIR)\MixerFader.obj"
	-@erase "$(INTDIR)\MixerFader.sbr"
	-@erase "$(INTDIR)\MyBitmap.obj"
	-@erase "$(INTDIR)\MyBitmap.sbr"
	-@erase "$(INTDIR)\ParticleBillbord.obj"
	-@erase "$(INTDIR)\ParticleBillbord.sbr"
	-@erase "$(INTDIR)\ParticleEmitter.obj"
	-@erase "$(INTDIR)\ParticleEmitter.sbr"
	-@erase "$(INTDIR)\ParticleMgr.obj"
	-@erase "$(INTDIR)\ParticleMgr.sbr"
	-@erase "$(INTDIR)\Particlesys.obj"
	-@erase "$(INTDIR)\Particlesys.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\sucollide.obj"
	-@erase "$(INTDIR)\sucollide.sbr"
	-@erase "$(INTDIR)\sumath.obj"
	-@erase "$(INTDIR)\sumath.sbr"
	-@erase "$(INTDIR)\SuMixerVolume.obj"
	-@erase "$(INTDIR)\SuMixerVolume.sbr"
	-@erase "$(INTDIR)\SuMP3Play.obj"
	-@erase "$(INTDIR)\SuMP3Play.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\X3DObject.obj"
	-@erase "$(INTDIR)\X3DObject.sbr"
	-@erase "$(INTDIR)\X3DObjectManager.obj"
	-@erase "$(INTDIR)\X3DObjectManager.sbr"
	-@erase "$(INTDIR)\X3DPickedPrimitive.obj"
	-@erase "$(INTDIR)\X3DPickedPrimitive.sbr"
	-@erase "$(INTDIR)\X3DUFont.obj"
	-@erase "$(INTDIR)\X3DUFont.sbr"
	-@erase "$(INTDIR)\X3PCamera.obj"
	-@erase "$(INTDIR)\X3PCamera.sbr"
	-@erase "$(INTDIR)\XAnimationController.obj"
	-@erase "$(INTDIR)\XAnimationController.sbr"
	-@erase "$(INTDIR)\XBallGauge.obj"
	-@erase "$(INTDIR)\XBallGauge.sbr"
	-@erase "$(INTDIR)\XBillboard.obj"
	-@erase "$(INTDIR)\XBillboard.sbr"
	-@erase "$(INTDIR)\XBipedController.obj"
	-@erase "$(INTDIR)\XBipedController.sbr"
	-@erase "$(INTDIR)\XButton.obj"
	-@erase "$(INTDIR)\XButton.sbr"
	-@erase "$(INTDIR)\XChatEdit.obj"
	-@erase "$(INTDIR)\XChatEdit.sbr"
	-@erase "$(INTDIR)\XCheckButton.obj"
	-@erase "$(INTDIR)\XCheckButton.sbr"
	-@erase "$(INTDIR)\XCompress.obj"
	-@erase "$(INTDIR)\XCompress.sbr"
	-@erase "$(INTDIR)\XDprintf.obj"
	-@erase "$(INTDIR)\XDprintf.sbr"
	-@erase "$(INTDIR)\XFastCode.obj"
	-@erase "$(INTDIR)\XFastCode.sbr"
	-@erase "$(INTDIR)\XGUIControlCenter.obj"
	-@erase "$(INTDIR)\XGUIControlCenter.sbr"
	-@erase "$(INTDIR)\XGUIObject.obj"
	-@erase "$(INTDIR)\XGUIObject.sbr"
	-@erase "$(INTDIR)\XHSlider.obj"
	-@erase "$(INTDIR)\XHSlider.sbr"
	-@erase "$(INTDIR)\XImageStatic.obj"
	-@erase "$(INTDIR)\XImageStatic.sbr"
	-@erase "$(INTDIR)\Xitem.obj"
	-@erase "$(INTDIR)\Xitem.sbr"
	-@erase "$(INTDIR)\XLensFlare.obj"
	-@erase "$(INTDIR)\XLensFlare.sbr"
	-@erase "$(INTDIR)\Xlist.obj"
	-@erase "$(INTDIR)\Xlist.sbr"
	-@erase "$(INTDIR)\XListBox.obj"
	-@erase "$(INTDIR)\XListBox.sbr"
	-@erase "$(INTDIR)\XLodTerrain.obj"
	-@erase "$(INTDIR)\XLodTerrain.sbr"
	-@erase "$(INTDIR)\XMeshContainer.obj"
	-@erase "$(INTDIR)\XMeshContainer.sbr"
	-@erase "$(INTDIR)\XMeshMath.obj"
	-@erase "$(INTDIR)\XMeshMath.sbr"
	-@erase "$(INTDIR)\XMessageBox.obj"
	-@erase "$(INTDIR)\XMessageBox.sbr"
	-@erase "$(INTDIR)\XMob.obj"
	-@erase "$(INTDIR)\XMob.sbr"
	-@erase "$(INTDIR)\XModelContainer.obj"
	-@erase "$(INTDIR)\XModelContainer.sbr"
	-@erase "$(INTDIR)\XMString.obj"
	-@erase "$(INTDIR)\XMString.sbr"
	-@erase "$(INTDIR)\XNetwork.obj"
	-@erase "$(INTDIR)\XNetwork.sbr"
	-@erase "$(INTDIR)\XNPC.obj"
	-@erase "$(INTDIR)\XNPC.sbr"
	-@erase "$(INTDIR)\XNPCScript.obj"
	-@erase "$(INTDIR)\XNPCScript.sbr"
	-@erase "$(INTDIR)\XObjectManager.obj"
	-@erase "$(INTDIR)\XObjectManager.sbr"
	-@erase "$(INTDIR)\XPackage.obj"
	-@erase "$(INTDIR)\XPackage.sbr"
	-@erase "$(INTDIR)\XPathFinder.obj"
	-@erase "$(INTDIR)\XPathFinder.sbr"
	-@erase "$(INTDIR)\XProc_CreateCharacter.obj"
	-@erase "$(INTDIR)\XProc_CreateCharacter.sbr"
	-@erase "$(INTDIR)\XProc_FirstLoad.obj"
	-@erase "$(INTDIR)\XProc_FirstLoad.sbr"
	-@erase "$(INTDIR)\XProc_Loading.obj"
	-@erase "$(INTDIR)\XProc_Loading.sbr"
	-@erase "$(INTDIR)\XProc_LoginServer.obj"
	-@erase "$(INTDIR)\XProc_LoginServer.sbr"
	-@erase "$(INTDIR)\XProc_MainGame.obj"
	-@erase "$(INTDIR)\XProc_MainGame.sbr"
	-@erase "$(INTDIR)\XProc_SelectCharacter.obj"
	-@erase "$(INTDIR)\XProc_SelectCharacter.sbr"
	-@erase "$(INTDIR)\XProcess.obj"
	-@erase "$(INTDIR)\XProcess.sbr"
	-@erase "$(INTDIR)\XQuestScriptItem.obj"
	-@erase "$(INTDIR)\XQuestScriptItem.sbr"
	-@erase "$(INTDIR)\XRegistry.obj"
	-@erase "$(INTDIR)\XRegistry.sbr"
	-@erase "$(INTDIR)\XRenderStateControl.obj"
	-@erase "$(INTDIR)\XRenderStateControl.sbr"
	-@erase "$(INTDIR)\XScrollBar.obj"
	-@erase "$(INTDIR)\XScrollBar.sbr"
	-@erase "$(INTDIR)\XSimpleEdit.obj"
	-@erase "$(INTDIR)\XSimpleEdit.sbr"
	-@erase "$(INTDIR)\XSoundKernel.obj"
	-@erase "$(INTDIR)\XSoundKernel.sbr"
	-@erase "$(INTDIR)\XStatic.obj"
	-@erase "$(INTDIR)\XStatic.sbr"
	-@erase "$(INTDIR)\XStringItem.obj"
	-@erase "$(INTDIR)\XStringItem.sbr"
	-@erase "$(INTDIR)\XTextureManager.obj"
	-@erase "$(INTDIR)\XTextureManager.sbr"
	-@erase "$(INTDIR)\XTimeCounter.obj"
	-@erase "$(INTDIR)\XTimeCounter.sbr"
	-@erase "$(INTDIR)\XTimer.obj"
	-@erase "$(INTDIR)\XTimer.sbr"
	-@erase "$(INTDIR)\XToolTip.obj"
	-@erase "$(INTDIR)\XToolTip.sbr"
	-@erase "$(INTDIR)\XUser.obj"
	-@erase "$(INTDIR)\XUser.sbr"
	-@erase "$(INTDIR)\XVSlider.obj"
	-@erase "$(INTDIR)\XVSlider.sbr"
	-@erase "$(INTDIR)\XWindow.obj"
	-@erase "$(INTDIR)\XWindow.sbr"
	-@erase "$(INTDIR)\XWindow_ChattingDefault.obj"
	-@erase "$(INTDIR)\XWindow_ChattingDefault.sbr"
	-@erase "$(INTDIR)\XWindow_ChattingMinimize.obj"
	-@erase "$(INTDIR)\XWindow_ChattingMinimize.sbr"
	-@erase "$(INTDIR)\XWindow_CInfo.obj"
	-@erase "$(INTDIR)\XWindow_CInfo.sbr"
	-@erase "$(INTDIR)\XWindow_FSWindow.obj"
	-@erase "$(INTDIR)\XWindow_FSWindow.sbr"
	-@erase "$(INTDIR)\XWindow_Inventory.obj"
	-@erase "$(INTDIR)\XWindow_Inventory.sbr"
	-@erase "$(INTDIR)\XWindow_ListBase.obj"
	-@erase "$(INTDIR)\XWindow_ListBase.sbr"
	-@erase "$(INTDIR)\XWindow_ModelDesc.obj"
	-@erase "$(INTDIR)\XWindow_ModelDesc.sbr"
	-@erase "$(INTDIR)\XWindow_ModelView.obj"
	-@erase "$(INTDIR)\XWindow_ModelView.sbr"
	-@erase "$(INTDIR)\XWindow_NPCScript.obj"
	-@erase "$(INTDIR)\XWindow_NPCScript.sbr"
	-@erase "$(INTDIR)\XWindow_Option.obj"
	-@erase "$(INTDIR)\XWindow_Option.sbr"
	-@erase "$(INTDIR)\XWindow_Party.obj"
	-@erase "$(INTDIR)\XWindow_Party.sbr"
	-@erase "$(INTDIR)\XWindow_PCTrade.obj"
	-@erase "$(INTDIR)\XWindow_PCTrade.sbr"
	-@erase "$(INTDIR)\XWindow_QSlot.obj"
	-@erase "$(INTDIR)\XWindow_QSlot.sbr"
	-@erase "$(INTDIR)\XWindow_QuestWindow.obj"
	-@erase "$(INTDIR)\XWindow_QuestWindow.sbr"
	-@erase "$(INTDIR)\XWindow_Skill.obj"
	-@erase "$(INTDIR)\XWindow_Skill.sbr"
	-@erase "$(INTDIR)\XWindow_SkillTooltip.obj"
	-@erase "$(INTDIR)\XWindow_SkillTooltip.sbr"
	-@erase "$(INTDIR)\XWindow_WorldMinimap.obj"
	-@erase "$(INTDIR)\XWindow_WorldMinimap.sbr"
	-@erase "$(INTDIR)\XWindowManager.obj"
	-@erase "$(INTDIR)\XWindowManager.sbr"
	-@erase "$(OUTDIR)\EmperorOfDragons.bsc"
	-@erase ".\Game\EmperorOfDragons_M.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./ErrorHandler" /I "./XWControl" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Particle" /I "./Script" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_XMANAGER" /D "_XADMINISTRATORMODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\EmperorOfDragons.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\EmperorOfDragons.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\EmperorOfDragons.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\EmperorOfDragons.sbr" \
	"$(INTDIR)\XDprintf.sbr" \
	"$(INTDIR)\XProcess.sbr" \
	"$(INTDIR)\XProc_LoginServer.sbr" \
	"$(INTDIR)\XProc_CreateCharacter.sbr" \
	"$(INTDIR)\XProc_SelectCharacter.sbr" \
	"$(INTDIR)\XWindow_ChattingDefault.sbr" \
	"$(INTDIR)\XWindow_ChattingMinimize.sbr" \
	"$(INTDIR)\XWindow_CInfo.sbr" \
	"$(INTDIR)\XWindow_FSWindow.sbr" \
	"$(INTDIR)\XWindow_Inventory.sbr" \
	"$(INTDIR)\XWindow_ListBase.sbr" \
	"$(INTDIR)\XWindow_ModelDesc.sbr" \
	"$(INTDIR)\XWindow_ModelView.sbr" \
	"$(INTDIR)\XWindow_NPCScript.sbr" \
	"$(INTDIR)\XWindow_Option.sbr" \
	"$(INTDIR)\XWindow_Party.sbr" \
	"$(INTDIR)\XWindow_PCTrade.sbr" \
	"$(INTDIR)\XWindow_QSlot.sbr" \
	"$(INTDIR)\XWindow_QuestWindow.sbr" \
	"$(INTDIR)\XWindow_Skill.sbr" \
	"$(INTDIR)\XWindow_SkillTooltip.sbr" \
	"$(INTDIR)\XWindow_WorldMinimap.sbr" \
	"$(INTDIR)\XProc_Loading.sbr" \
	"$(INTDIR)\XProc_MainGame.sbr" \
	"$(INTDIR)\XProc_FirstLoad.sbr" \
	"$(INTDIR)\GenericWindowClass.sbr" \
	"$(INTDIR)\MyBitmap.sbr" \
	"$(INTDIR)\XChatEdit.sbr" \
	"$(INTDIR)\XSimpleEdit.sbr" \
	"$(INTDIR)\Xitem.sbr" \
	"$(INTDIR)\Xlist.sbr" \
	"$(INTDIR)\XMob.sbr" \
	"$(INTDIR)\XNPC.sbr" \
	"$(INTDIR)\XObjectManager.sbr" \
	"$(INTDIR)\XUser.sbr" \
	"$(INTDIR)\CsuX3D_Box.sbr" \
	"$(INTDIR)\sucollide.sbr" \
	"$(INTDIR)\sumath.sbr" \
	"$(INTDIR)\d3dfont.sbr" \
	"$(INTDIR)\d3dutil.sbr" \
	"$(INTDIR)\dsutil.sbr" \
	"$(INTDIR)\dxutil.sbr" \
	"$(INTDIR)\XCompress.sbr" \
	"$(INTDIR)\XFastCode.sbr" \
	"$(INTDIR)\XPackage.sbr" \
	"$(INTDIR)\XRegistry.sbr" \
	"$(INTDIR)\XStringItem.sbr" \
	"$(INTDIR)\XTimeCounter.sbr" \
	"$(INTDIR)\XTimer.sbr" \
	"$(INTDIR)\XNetwork.sbr" \
	"$(INTDIR)\XBillboard.sbr" \
	"$(INTDIR)\XLensFlare.sbr" \
	"$(INTDIR)\XBipedController.sbr" \
	"$(INTDIR)\XMeshContainer.sbr" \
	"$(INTDIR)\KUWater.sbr" \
	"$(INTDIR)\X3DObject.sbr" \
	"$(INTDIR)\X3DObjectManager.sbr" \
	"$(INTDIR)\X3DPickedPrimitive.sbr" \
	"$(INTDIR)\X3PCamera.sbr" \
	"$(INTDIR)\XAnimationController.sbr" \
	"$(INTDIR)\XLodTerrain.sbr" \
	"$(INTDIR)\XMeshMath.sbr" \
	"$(INTDIR)\XModelContainer.sbr" \
	"$(INTDIR)\XRenderStateControl.sbr" \
	"$(INTDIR)\XTextureManager.sbr" \
	"$(INTDIR)\CObs.sbr" \
	"$(INTDIR)\XPathFinder.sbr" \
	"$(INTDIR)\MixerBase.sbr" \
	"$(INTDIR)\MixerFader.sbr" \
	"$(INTDIR)\SuMixerVolume.sbr" \
	"$(INTDIR)\SuMP3Play.sbr" \
	"$(INTDIR)\XSoundKernel.sbr" \
	"$(INTDIR)\X3DUFont.sbr" \
	"$(INTDIR)\XBallGauge.sbr" \
	"$(INTDIR)\XButton.sbr" \
	"$(INTDIR)\XCheckButton.sbr" \
	"$(INTDIR)\XGUIControlCenter.sbr" \
	"$(INTDIR)\XGUIObject.sbr" \
	"$(INTDIR)\XHSlider.sbr" \
	"$(INTDIR)\XImageStatic.sbr" \
	"$(INTDIR)\XListBox.sbr" \
	"$(INTDIR)\XMessageBox.sbr" \
	"$(INTDIR)\XMString.sbr" \
	"$(INTDIR)\XScrollBar.sbr" \
	"$(INTDIR)\XStatic.sbr" \
	"$(INTDIR)\XToolTip.sbr" \
	"$(INTDIR)\XVSlider.sbr" \
	"$(INTDIR)\XWindow.sbr" \
	"$(INTDIR)\XWindowManager.sbr" \
	"$(INTDIR)\C3TimesEffect.sbr" \
	"$(INTDIR)\CArmsSignal.sbr" \
	"$(INTDIR)\CEffectBillboard.sbr" \
	"$(INTDIR)\CEffectBlood.sbr" \
	"$(INTDIR)\CEffectMgr.sbr" \
	"$(INTDIR)\CEffectShadow.sbr" \
	"$(INTDIR)\CFootHold.sbr" \
	"$(INTDIR)\CircleParticle.sbr" \
	"$(INTDIR)\CirclePattern.sbr" \
	"$(INTDIR)\CLevUpEffect.sbr" \
	"$(INTDIR)\CLevUpTypeB.sbr" \
	"$(INTDIR)\CommonFuncs.sbr" \
	"$(INTDIR)\ControlFunUtil.sbr" \
	"$(INTDIR)\ParticleBillbord.sbr" \
	"$(INTDIR)\ParticleEmitter.sbr" \
	"$(INTDIR)\ParticleMgr.sbr" \
	"$(INTDIR)\Particlesys.sbr" \
	"$(INTDIR)\XNPCScript.sbr" \
	"$(INTDIR)\XQuestScriptItem.sbr"

"$(OUTDIR)\EmperorOfDragons.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=./Library/XGamebase_M.lib ./Library/XKernel_M.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib d3dx8.lib dinput8.lib DxErr8.lib dsound.lib dxguid.lib ws2_32.lib imagehlp.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\EmperorOfDragons_M.pdb" /machine:I386 /out:"./Game/EmperorOfDragons_M.exe" 
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\EmperorOfDragons.obj" \
	"$(INTDIR)\XDprintf.obj" \
	"$(INTDIR)\XProcess.obj" \
	"$(INTDIR)\XProc_LoginServer.obj" \
	"$(INTDIR)\XProc_CreateCharacter.obj" \
	"$(INTDIR)\XProc_SelectCharacter.obj" \
	"$(INTDIR)\XWindow_ChattingDefault.obj" \
	"$(INTDIR)\XWindow_ChattingMinimize.obj" \
	"$(INTDIR)\XWindow_CInfo.obj" \
	"$(INTDIR)\XWindow_FSWindow.obj" \
	"$(INTDIR)\XWindow_Inventory.obj" \
	"$(INTDIR)\XWindow_ListBase.obj" \
	"$(INTDIR)\XWindow_ModelDesc.obj" \
	"$(INTDIR)\XWindow_ModelView.obj" \
	"$(INTDIR)\XWindow_NPCScript.obj" \
	"$(INTDIR)\XWindow_Option.obj" \
	"$(INTDIR)\XWindow_Party.obj" \
	"$(INTDIR)\XWindow_PCTrade.obj" \
	"$(INTDIR)\XWindow_QSlot.obj" \
	"$(INTDIR)\XWindow_QuestWindow.obj" \
	"$(INTDIR)\XWindow_Skill.obj" \
	"$(INTDIR)\XWindow_SkillTooltip.obj" \
	"$(INTDIR)\XWindow_WorldMinimap.obj" \
	"$(INTDIR)\XProc_Loading.obj" \
	"$(INTDIR)\XProc_MainGame.obj" \
	"$(INTDIR)\XProc_FirstLoad.obj" \
	"$(INTDIR)\GenericWindowClass.obj" \
	"$(INTDIR)\MyBitmap.obj" \
	"$(INTDIR)\XChatEdit.obj" \
	"$(INTDIR)\XSimpleEdit.obj" \
	"$(INTDIR)\Xitem.obj" \
	"$(INTDIR)\Xlist.obj" \
	"$(INTDIR)\XMob.obj" \
	"$(INTDIR)\XNPC.obj" \
	"$(INTDIR)\XObjectManager.obj" \
	"$(INTDIR)\XUser.obj" \
	"$(INTDIR)\CsuX3D_Box.obj" \
	"$(INTDIR)\sucollide.obj" \
	"$(INTDIR)\sumath.obj" \
	"$(INTDIR)\d3dfont.obj" \
	"$(INTDIR)\d3dutil.obj" \
	"$(INTDIR)\dsutil.obj" \
	"$(INTDIR)\dxutil.obj" \
	"$(INTDIR)\XCompress.obj" \
	"$(INTDIR)\XFastCode.obj" \
	"$(INTDIR)\XPackage.obj" \
	"$(INTDIR)\XRegistry.obj" \
	"$(INTDIR)\XStringItem.obj" \
	"$(INTDIR)\XTimeCounter.obj" \
	"$(INTDIR)\XTimer.obj" \
	"$(INTDIR)\XNetwork.obj" \
	"$(INTDIR)\XBillboard.obj" \
	"$(INTDIR)\XLensFlare.obj" \
	"$(INTDIR)\XBipedController.obj" \
	"$(INTDIR)\XMeshContainer.obj" \
	"$(INTDIR)\KUWater.obj" \
	"$(INTDIR)\X3DObject.obj" \
	"$(INTDIR)\X3DObjectManager.obj" \
	"$(INTDIR)\X3DPickedPrimitive.obj" \
	"$(INTDIR)\X3PCamera.obj" \
	"$(INTDIR)\XAnimationController.obj" \
	"$(INTDIR)\XLodTerrain.obj" \
	"$(INTDIR)\XMeshMath.obj" \
	"$(INTDIR)\XModelContainer.obj" \
	"$(INTDIR)\XRenderStateControl.obj" \
	"$(INTDIR)\XTextureManager.obj" \
	"$(INTDIR)\CObs.obj" \
	"$(INTDIR)\XPathFinder.obj" \
	"$(INTDIR)\MixerBase.obj" \
	"$(INTDIR)\MixerFader.obj" \
	"$(INTDIR)\SuMixerVolume.obj" \
	"$(INTDIR)\SuMP3Play.obj" \
	"$(INTDIR)\XSoundKernel.obj" \
	"$(INTDIR)\X3DUFont.obj" \
	"$(INTDIR)\XBallGauge.obj" \
	"$(INTDIR)\XButton.obj" \
	"$(INTDIR)\XCheckButton.obj" \
	"$(INTDIR)\XGUIControlCenter.obj" \
	"$(INTDIR)\XGUIObject.obj" \
	"$(INTDIR)\XHSlider.obj" \
	"$(INTDIR)\XImageStatic.obj" \
	"$(INTDIR)\XListBox.obj" \
	"$(INTDIR)\XMessageBox.obj" \
	"$(INTDIR)\XMString.obj" \
	"$(INTDIR)\XScrollBar.obj" \
	"$(INTDIR)\XStatic.obj" \
	"$(INTDIR)\XToolTip.obj" \
	"$(INTDIR)\XVSlider.obj" \
	"$(INTDIR)\XWindow.obj" \
	"$(INTDIR)\XWindowManager.obj" \
	"$(INTDIR)\C3TimesEffect.obj" \
	"$(INTDIR)\CArmsSignal.obj" \
	"$(INTDIR)\CEffectBillboard.obj" \
	"$(INTDIR)\CEffectBlood.obj" \
	"$(INTDIR)\CEffectMgr.obj" \
	"$(INTDIR)\CEffectShadow.obj" \
	"$(INTDIR)\CFootHold.obj" \
	"$(INTDIR)\CircleParticle.obj" \
	"$(INTDIR)\CirclePattern.obj" \
	"$(INTDIR)\CLevUpEffect.obj" \
	"$(INTDIR)\CLevUpTypeB.obj" \
	"$(INTDIR)\CommonFuncs.obj" \
	"$(INTDIR)\ControlFunUtil.obj" \
	"$(INTDIR)\ParticleBillbord.obj" \
	"$(INTDIR)\ParticleEmitter.obj" \
	"$(INTDIR)\ParticleMgr.obj" \
	"$(INTDIR)\Particlesys.obj" \
	"$(INTDIR)\XNPCScript.obj" \
	"$(INTDIR)\XQuestScriptItem.obj" \
	"$(INTDIR)\EmperorOfDragons.res" \
	".\Library\XGamebase_M.lib" \
	".\Library\XKernel_M.lib"

".\Game\EmperorOfDragons_M.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Compiled/releasemanager/EOD
# End Custom Macros

$(DS_POSTBUILD_DEP) : "XKernel - Win32 Release_Manager" "XGamebase - Win32 Release_Manager" ".\Game\EmperorOfDragons_M.exe" "$(OUTDIR)\EmperorOfDragons.bsc"
   rem copy .\Game\EmperorOfDragons_M.exe z:
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("EmperorOfDragons.dep")
!INCLUDE "EmperorOfDragons.dep"
!ELSE 
!MESSAGE Warning: cannot find "EmperorOfDragons.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "EmperorOfDragons - Win32 Release" || "$(CFG)" == "EmperorOfDragons - Win32 Debug" || "$(CFG)" == "EmperorOfDragons - Win32 Release_include_debugging_information" || "$(CFG)" == "EmperorOfDragons - Win32 Release_Manager"
SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "EmperorOfDragons - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./ErrorHandler" /I "./XWControl" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Particle" /I "./Script" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\EmperorOfDragons.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\EmperorOfDragons.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./ErrorHandler" /I "./XWControl" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Particle" /I "./Script" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\EmperorOfDragons.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\EmperorOfDragons.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 Release_include_debugging_information"

CPP_SWITCHES=/nologo /ML /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./ErrorHandler" /I "./XWControl" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Particle" /I "./Script" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /FAs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\EmperorOfDragons.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\EmperorOfDragons.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 Release_Manager"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./ErrorHandler" /I "./XWControl" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Particle" /I "./Script" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_XMANAGER" /D "_XADMINISTRATORMODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\EmperorOfDragons.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\EmperorOfDragons.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\EmperorOfDragons.rc

"$(INTDIR)\EmperorOfDragons.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\EmperorOfDragons.cpp

"$(INTDIR)\EmperorOfDragons.obj"	"$(INTDIR)\EmperorOfDragons.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"


SOURCE=.\ErrorHandler\XDprintf.cpp

"$(INTDIR)\XDprintf.obj"	"$(INTDIR)\XDprintf.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XProcess.cpp

"$(INTDIR)\XProcess.obj"	"$(INTDIR)\XProcess.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XProc_LoginServer.cpp

"$(INTDIR)\XProc_LoginServer.obj"	"$(INTDIR)\XProc_LoginServer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XProc_CreateCharacter.cpp

"$(INTDIR)\XProc_CreateCharacter.obj"	"$(INTDIR)\XProc_CreateCharacter.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XProc_SelectCharacter.cpp

"$(INTDIR)\XProc_SelectCharacter.obj"	"$(INTDIR)\XProc_SelectCharacter.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_ChattingDefault.cpp

"$(INTDIR)\XWindow_ChattingDefault.obj"	"$(INTDIR)\XWindow_ChattingDefault.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_ChattingMinimize.cpp

"$(INTDIR)\XWindow_ChattingMinimize.obj"	"$(INTDIR)\XWindow_ChattingMinimize.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_CInfo.cpp

"$(INTDIR)\XWindow_CInfo.obj"	"$(INTDIR)\XWindow_CInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_FSWindow.cpp

"$(INTDIR)\XWindow_FSWindow.obj"	"$(INTDIR)\XWindow_FSWindow.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_Inventory.cpp

"$(INTDIR)\XWindow_Inventory.obj"	"$(INTDIR)\XWindow_Inventory.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_ListBase.cpp

"$(INTDIR)\XWindow_ListBase.obj"	"$(INTDIR)\XWindow_ListBase.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_ModelDesc.cpp

"$(INTDIR)\XWindow_ModelDesc.obj"	"$(INTDIR)\XWindow_ModelDesc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_ModelView.cpp

"$(INTDIR)\XWindow_ModelView.obj"	"$(INTDIR)\XWindow_ModelView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_NPCScript.cpp

"$(INTDIR)\XWindow_NPCScript.obj"	"$(INTDIR)\XWindow_NPCScript.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_Option.cpp

"$(INTDIR)\XWindow_Option.obj"	"$(INTDIR)\XWindow_Option.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_Party.cpp

"$(INTDIR)\XWindow_Party.obj"	"$(INTDIR)\XWindow_Party.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_PCTrade.cpp

"$(INTDIR)\XWindow_PCTrade.obj"	"$(INTDIR)\XWindow_PCTrade.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_QSlot.cpp

"$(INTDIR)\XWindow_QSlot.obj"	"$(INTDIR)\XWindow_QSlot.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_QuestWindow.cpp

"$(INTDIR)\XWindow_QuestWindow.obj"	"$(INTDIR)\XWindow_QuestWindow.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_Skill.cpp

"$(INTDIR)\XWindow_Skill.obj"	"$(INTDIR)\XWindow_Skill.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_SkillTooltip.cpp

"$(INTDIR)\XWindow_SkillTooltip.obj"	"$(INTDIR)\XWindow_SkillTooltip.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XWindow_WorldMinimap.cpp

"$(INTDIR)\XWindow_WorldMinimap.obj"	"$(INTDIR)\XWindow_WorldMinimap.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XProc_Loading.cpp

"$(INTDIR)\XProc_Loading.obj"	"$(INTDIR)\XProc_Loading.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XProc_MainGame.cpp

"$(INTDIR)\XProc_MainGame.obj"	"$(INTDIR)\XProc_MainGame.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XProcess\XProc_FirstLoad.cpp

"$(INTDIR)\XProc_FirstLoad.obj"	"$(INTDIR)\XProc_FirstLoad.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XWControl\GenericWindowClass.cpp

"$(INTDIR)\GenericWindowClass.obj"	"$(INTDIR)\GenericWindowClass.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XWControl\MyBitmap.cpp

"$(INTDIR)\MyBitmap.obj"	"$(INTDIR)\MyBitmap.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XWControl\XChatEdit.cpp

"$(INTDIR)\XChatEdit.obj"	"$(INTDIR)\XChatEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XWControl\XSimpleEdit.cpp

"$(INTDIR)\XSimpleEdit.obj"	"$(INTDIR)\XSimpleEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\DataStructure\Xitem.cpp

"$(INTDIR)\Xitem.obj"	"$(INTDIR)\Xitem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\DataStructure\Xlist.cpp

"$(INTDIR)\Xlist.obj"	"$(INTDIR)\Xlist.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\DataStructure\XMob.cpp

"$(INTDIR)\XMob.obj"	"$(INTDIR)\XMob.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\DataStructure\XNPC.cpp

"$(INTDIR)\XNPC.obj"	"$(INTDIR)\XNPC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ObjectControl\XObjectManager.cpp

"$(INTDIR)\XObjectManager.obj"	"$(INTDIR)\XObjectManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\DataStructure\XUser.cpp

"$(INTDIR)\XUser.obj"	"$(INTDIR)\XUser.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\MeshControl\CsuX3D_Box.cpp

"$(INTDIR)\CsuX3D_Box.obj"	"$(INTDIR)\CsuX3D_Box.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Utility\sucollide.cpp

"$(INTDIR)\sucollide.obj"	"$(INTDIR)\sucollide.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Utility\sumath.cpp

"$(INTDIR)\sumath.obj"	"$(INTDIR)\sumath.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Utility\d3dfont.cpp

"$(INTDIR)\d3dfont.obj"	"$(INTDIR)\d3dfont.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Utility\d3dutil.cpp

"$(INTDIR)\d3dutil.obj"	"$(INTDIR)\d3dutil.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Utility\dsutil.cpp

"$(INTDIR)\dsutil.obj"	"$(INTDIR)\dsutil.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Utility\dxutil.cpp

"$(INTDIR)\dxutil.obj"	"$(INTDIR)\dxutil.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Utility\XCompress.cpp

"$(INTDIR)\XCompress.obj"	"$(INTDIR)\XCompress.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Utility\XFastCode.cpp

"$(INTDIR)\XFastCode.obj"	"$(INTDIR)\XFastCode.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Utility\XPackage.cpp

"$(INTDIR)\XPackage.obj"	"$(INTDIR)\XPackage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Utility\XRegistry.cpp

"$(INTDIR)\XRegistry.obj"	"$(INTDIR)\XRegistry.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Utility\XStringItem.cpp

"$(INTDIR)\XStringItem.obj"	"$(INTDIR)\XStringItem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Utility\XTimeCounter.cpp

"$(INTDIR)\XTimeCounter.obj"	"$(INTDIR)\XTimeCounter.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Utility\XTimer.cpp

"$(INTDIR)\XTimer.obj"	"$(INTDIR)\XTimer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Network\XNetwork.cpp

"$(INTDIR)\XNetwork.obj"	"$(INTDIR)\XNetwork.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\MeshControl\XBillboard.cpp

"$(INTDIR)\XBillboard.obj"	"$(INTDIR)\XBillboard.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\MeshControl\XLensFlare.cpp

"$(INTDIR)\XLensFlare.obj"	"$(INTDIR)\XLensFlare.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\MeshControl\XBipedController.cpp

"$(INTDIR)\XBipedController.obj"	"$(INTDIR)\XBipedController.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\MeshControl\XMeshContainer.cpp

"$(INTDIR)\XMeshContainer.obj"	"$(INTDIR)\XMeshContainer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\MeshControl\KUWater.cpp

"$(INTDIR)\KUWater.obj"	"$(INTDIR)\KUWater.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\X3DObject.cpp

"$(INTDIR)\X3DObject.obj"	"$(INTDIR)\X3DObject.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\X3DObjectManager.cpp

"$(INTDIR)\X3DObjectManager.obj"	"$(INTDIR)\X3DObjectManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\MeshControl\X3DPickedPrimitive.cpp

"$(INTDIR)\X3DPickedPrimitive.obj"	"$(INTDIR)\X3DPickedPrimitive.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\X3PCamera.cpp

"$(INTDIR)\X3PCamera.obj"	"$(INTDIR)\X3PCamera.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\MeshControl\XAnimationController.cpp

"$(INTDIR)\XAnimationController.obj"	"$(INTDIR)\XAnimationController.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\MeshControl\XLodTerrain.cpp

"$(INTDIR)\XLodTerrain.obj"	"$(INTDIR)\XLodTerrain.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\MeshControl\XMeshMath.cpp

"$(INTDIR)\XMeshMath.obj"	"$(INTDIR)\XMeshMath.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\MeshControl\XModelContainer.cpp

"$(INTDIR)\XModelContainer.obj"	"$(INTDIR)\XModelContainer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\MeshControl\XRenderStateControl.cpp

"$(INTDIR)\XRenderStateControl.obj"	"$(INTDIR)\XRenderStateControl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\MeshControl\XTextureManager.cpp

"$(INTDIR)\XTextureManager.obj"	"$(INTDIR)\XTextureManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\PathFinding\CObs.cpp

"$(INTDIR)\CObs.obj"	"$(INTDIR)\CObs.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\DataStructure\XPathFinder.cpp

"$(INTDIR)\XPathFinder.obj"	"$(INTDIR)\XPathFinder.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XSound\MixerBase.cpp

"$(INTDIR)\MixerBase.obj"	"$(INTDIR)\MixerBase.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XSound\MixerFader.cpp

"$(INTDIR)\MixerFader.obj"	"$(INTDIR)\MixerFader.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XSound\SuMixerVolume.cpp

"$(INTDIR)\SuMixerVolume.obj"	"$(INTDIR)\SuMixerVolume.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XSound\SuMP3Play.cpp

"$(INTDIR)\SuMP3Play.obj"	"$(INTDIR)\SuMP3Play.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XSound\XSoundKernel.cpp

"$(INTDIR)\XSoundKernel.obj"	"$(INTDIR)\XSoundKernel.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Utility\X3DUFont.cpp

"$(INTDIR)\X3DUFont.obj"	"$(INTDIR)\X3DUFont.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\XBallGauge.cpp

"$(INTDIR)\XBallGauge.obj"	"$(INTDIR)\XBallGauge.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\XButton.cpp

"$(INTDIR)\XButton.obj"	"$(INTDIR)\XButton.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\XCheckButton.cpp

"$(INTDIR)\XCheckButton.obj"	"$(INTDIR)\XCheckButton.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\XGUIControlCenter.cpp

"$(INTDIR)\XGUIControlCenter.obj"	"$(INTDIR)\XGUIControlCenter.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\XGUIObject.cpp

"$(INTDIR)\XGUIObject.obj"	"$(INTDIR)\XGUIObject.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\XHSlider.cpp

"$(INTDIR)\XHSlider.obj"	"$(INTDIR)\XHSlider.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\XImageStatic.cpp

"$(INTDIR)\XImageStatic.obj"	"$(INTDIR)\XImageStatic.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\XListBox.cpp

"$(INTDIR)\XListBox.obj"	"$(INTDIR)\XListBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\XMessageBox.cpp

"$(INTDIR)\XMessageBox.obj"	"$(INTDIR)\XMessageBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\XMString.cpp

"$(INTDIR)\XMString.obj"	"$(INTDIR)\XMString.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\XScrollBar.cpp

"$(INTDIR)\XScrollBar.obj"	"$(INTDIR)\XScrollBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\XStatic.cpp

"$(INTDIR)\XStatic.obj"	"$(INTDIR)\XStatic.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\XToolTip.cpp

"$(INTDIR)\XToolTip.obj"	"$(INTDIR)\XToolTip.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\XVSlider.cpp

"$(INTDIR)\XVSlider.obj"	"$(INTDIR)\XVSlider.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\XWindow.cpp

"$(INTDIR)\XWindow.obj"	"$(INTDIR)\XWindow.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\XControl\XWindowManager.cpp

"$(INTDIR)\XWindowManager.obj"	"$(INTDIR)\XWindowManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\C3TimesEffect.cpp

"$(INTDIR)\C3TimesEffect.obj"	"$(INTDIR)\C3TimesEffect.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\CArmsSignal.cpp

"$(INTDIR)\CArmsSignal.obj"	"$(INTDIR)\CArmsSignal.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\CEffectBillboard.cpp

"$(INTDIR)\CEffectBillboard.obj"	"$(INTDIR)\CEffectBillboard.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\CEffectBlood.cpp

"$(INTDIR)\CEffectBlood.obj"	"$(INTDIR)\CEffectBlood.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\CEffectMgr.cpp

"$(INTDIR)\CEffectMgr.obj"	"$(INTDIR)\CEffectMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\CEffectShadow.cpp

"$(INTDIR)\CEffectShadow.obj"	"$(INTDIR)\CEffectShadow.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\CFootHold.cpp

"$(INTDIR)\CFootHold.obj"	"$(INTDIR)\CFootHold.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\CircleParticle.cpp

"$(INTDIR)\CircleParticle.obj"	"$(INTDIR)\CircleParticle.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\CirclePattern.cpp

"$(INTDIR)\CirclePattern.obj"	"$(INTDIR)\CirclePattern.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\CLevUpEffect.cpp

"$(INTDIR)\CLevUpEffect.obj"	"$(INTDIR)\CLevUpEffect.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\CLevUpTypeB.cpp

"$(INTDIR)\CLevUpTypeB.obj"	"$(INTDIR)\CLevUpTypeB.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\CommonFuncs.cpp

"$(INTDIR)\CommonFuncs.obj"	"$(INTDIR)\CommonFuncs.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\ControlFunUtil.cpp

"$(INTDIR)\ControlFunUtil.obj"	"$(INTDIR)\ControlFunUtil.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\ParticleBillbord.cpp

"$(INTDIR)\ParticleBillbord.obj"	"$(INTDIR)\ParticleBillbord.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\ParticleEmitter.cpp

"$(INTDIR)\ParticleEmitter.obj"	"$(INTDIR)\ParticleEmitter.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\ParticleMgr.cpp

"$(INTDIR)\ParticleMgr.obj"	"$(INTDIR)\ParticleMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Particle\Particlesys.cpp

"$(INTDIR)\Particlesys.obj"	"$(INTDIR)\Particlesys.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Script\XNPCScript.cpp

"$(INTDIR)\XNPCScript.obj"	"$(INTDIR)\XNPCScript.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Script\XQuestScriptItem.cpp

"$(INTDIR)\XQuestScriptItem.obj"	"$(INTDIR)\XQuestScriptItem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EmperorOfDragons.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "EmperorOfDragons - Win32 Release"

"XGamebase - Win32 Release" : 
   cd ".\XGamebase"
   $(MAKE) /$(MAKEFLAGS) /F .\XGamebase.mak CFG="XGamebase - Win32 Release" 
   cd ".."

"XGamebase - Win32 ReleaseCLEAN" : 
   cd ".\XGamebase"
   $(MAKE) /$(MAKEFLAGS) /F .\XGamebase.mak CFG="XGamebase - Win32 Release" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 Debug"

"XGamebase - Win32 Debug" : 
   cd ".\XGamebase"
   $(MAKE) /$(MAKEFLAGS) /F .\XGamebase.mak CFG="XGamebase - Win32 Debug" 
   cd ".."

"XGamebase - Win32 DebugCLEAN" : 
   cd ".\XGamebase"
   $(MAKE) /$(MAKEFLAGS) /F .\XGamebase.mak CFG="XGamebase - Win32 Debug" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 Release_include_debugging_information"

"XGamebase - Win32 Release_include_debugging_information" : 
   cd ".\XGamebase"
   $(MAKE) /$(MAKEFLAGS) /F .\XGamebase.mak CFG="XGamebase - Win32 Release_include_debugging_information" 
   cd ".."

"XGamebase - Win32 Release_include_debugging_informationCLEAN" : 
   cd ".\XGamebase"
   $(MAKE) /$(MAKEFLAGS) /F .\XGamebase.mak CFG="XGamebase - Win32 Release_include_debugging_information" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 Release_Manager"

"XGamebase - Win32 Release_Manager" : 
   cd ".\XGamebase"
   $(MAKE) /$(MAKEFLAGS) /F .\XGamebase.mak CFG="XGamebase - Win32 Release_Manager" 
   cd ".."

"XGamebase - Win32 Release_ManagerCLEAN" : 
   cd ".\XGamebase"
   $(MAKE) /$(MAKEFLAGS) /F .\XGamebase.mak CFG="XGamebase - Win32 Release_Manager" RECURSE=1 CLEAN 
   cd ".."

!ENDIF 

!IF  "$(CFG)" == "EmperorOfDragons - Win32 Release"

"XKernel - Win32 Release" : 
   cd ".\XKernel"
   $(MAKE) /$(MAKEFLAGS) /F .\XKernel.mak CFG="XKernel - Win32 Release" 
   cd ".."

"XKernel - Win32 ReleaseCLEAN" : 
   cd ".\XKernel"
   $(MAKE) /$(MAKEFLAGS) /F .\XKernel.mak CFG="XKernel - Win32 Release" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 Debug"

"XKernel - Win32 Debug" : 
   cd ".\XKernel"
   $(MAKE) /$(MAKEFLAGS) /F .\XKernel.mak CFG="XKernel - Win32 Debug" 
   cd ".."

"XKernel - Win32 DebugCLEAN" : 
   cd ".\XKernel"
   $(MAKE) /$(MAKEFLAGS) /F .\XKernel.mak CFG="XKernel - Win32 Debug" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 Release_include_debugging_information"

"XKernel - Win32 Release_include_debugging_information" : 
   cd ".\XKernel"
   $(MAKE) /$(MAKEFLAGS) /F .\XKernel.mak CFG="XKernel - Win32 Release_include_debugging_information" 
   cd ".."

"XKernel - Win32 Release_include_debugging_informationCLEAN" : 
   cd ".\XKernel"
   $(MAKE) /$(MAKEFLAGS) /F .\XKernel.mak CFG="XKernel - Win32 Release_include_debugging_information" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 Release_Manager"

"XKernel - Win32 Release_Manager" : 
   cd ".\XKernel"
   $(MAKE) /$(MAKEFLAGS) /F .\XKernel.mak CFG="XKernel - Win32 Release_Manager" 
   cd ".."

"XKernel - Win32 Release_ManagerCLEAN" : 
   cd ".\XKernel"
   $(MAKE) /$(MAKEFLAGS) /F .\XKernel.mak CFG="XKernel - Win32 Release_Manager" RECURSE=1 CLEAN 
   cd ".."

!ENDIF 


!ENDIF 

