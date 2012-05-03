#pragma once

struct _sHeader ;

#define MSG_NO_COLLECT_REQ				159		// 명품 혹은 수집품 교환요청
#define MSG_NO_COLLECT_RES				MSG_NO_COLLECT_REQ	//몇픔 곳은 수집품 교환 결과
//MSG_NO_COLLECT_REQ				159		// 명품 혹은 수집품 교환요청
struct MSG_COLLECT_REQ
{
	_sHeader			header;
	u_char				ucMessage;

	//현재 패킷의 타잎
	u_char				ucPacketType;	//0: 자원 보상, 1: 명품 save, 2: 명품 보상

	u_short				usIndex;		//자원 혹은 명품 인덱스

	u_char				ucSlotIndex;	//자원 혹은 명품의 슬롯 인덱스(명품 save시에만 사용)
};

//MSG_NO_COLLECT_RES				MSG_NO_COLLECT_REQ	//몇픔 곳은 수집품 교환 결과
struct MSG_COLLECT_RES
{
	_sHeader			header;
	u_char				ucMessage;

	//현재 패킷의 타잎
	u_char				ucPacketType;	//0: 자원 보상, 1: 명품 save, 2: 명품 보상
	u_char				ucFlag;			//0: 실패, 1: 성공
};

// 수집 패킷 변경

#define MAX_MATERIAL_RESOURCE_ITEM			16	// 재료 수집 최대 개수

struct MSG_COLLECT_PACKET 
{
	_sHeader			header;
	u_char				ucMessage;

	u_char				ucMode;		// en_collect_mode..
};

enum
{
	en_collect_mode_complete_resource = 0,		// 자원 보상
	en_collect_mode_save_collection = 1,		// 명품 저장
	en_collect_mode_complete_collection = 2,	// 명품 보상

	en_collect_mode_result = 10
};

// en_collect_mode_complete_resource		// 자원 보상 (재료수집)
struct MSG_COLLECT_COMPLETE_RESOURCE : public MSG_COLLECT_PACKET
{
	u_short				usIndex;			//자원 혹은 명품 인덱스
	u_char				item_slot_index[MAX_MATERIAL_RESOURCE_ITEM];	// 재료 아이템의 인벤토리 슬롯 인덱스
};

// en_collect_mode_save_collection			// 명품 저장
struct MSG_COLLECT_SAVE_COLLECTION : public MSG_COLLECT_PACKET
{
	u_short				usIndex;				//자원 혹은 명품 인덱스
	u_char				ucCollectionSlotIndex;	// 명품의 재료 목록 인덱스 
	u_char				ucInvenSlotIndex;		// 재료가 있는 인벤토리 인덱스
};

// en_collect_mode_complete_collection = 2	// 명품 보상
struct MSG_COLLECT_COMPLETE_COLLECTION : public MSG_COLLECT_PACKET
{
	u_short				usIndex;
	u_char				ucSlotIndex;
};

// en_collect_mode_result = 10
struct MSG_COLLECT_RESULT : public MSG_COLLECT_PACKET
{
	u_char				ucReqMode;		// 어떤 요청의 결과 인가
	u_char				ucFail;			// 0:fail   1:success
};