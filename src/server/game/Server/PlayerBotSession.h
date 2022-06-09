
#ifndef __PlayerBotSESSION_H
#define __PlayerBotSESSION_H

#include "WorldSession.h"
#include "PlayerBotMgr.h"
#include <mutex>

#define CAST_SCHEDULE_TICK 300

enum BotGlobleScheduleType
{
	BGSType_Online,
	BGSType_Offline,
	BGSType_Settting,
	BGSType_InBGQueue,
	BGSType_OutBGQueue,
	BGSType_EnterBG,
	BGSType_LeaveBG,
	BGSType_InAAQueue,
	BGSType_OutAAQueue,
	BGSType_EnterAA,
	BGSType_LeaveAA,
	BGSType_Online_GUID,
	BGSType_DelayLevelup,
	BGSType_InLFGQueue,
	BGSType_OutLFGQueue,
	BGSType_AcceptLFGProposal,
	BGSType_OfferPetitionSign
};
struct BotGlobleSchedule
{
	BotGlobleScheduleType bbgType;
	ObjectGuid playerGUID;
	uint32 parameter1;
	uint32 parameter2;
	uint32 parameter3;
	uint32 parameter4;
	uint32 parameter5;

	uint32 scheduleState;
	int32 processTick;

    BotGlobleSchedule(BotGlobleScheduleType eType, const ObjectGuid& guid) :
        bbgType(eType), playerGUID(guid)
    {
        parameter1 = 0;
        parameter2 = 0;
        parameter3 = 0;
        parameter4 = 0;
        parameter5 = 0;

        scheduleState = 0;
        processTick = 0;
    }

	BotGlobleSchedule(BotGlobleScheduleType eType, uint64 guid) :
		bbgType(eType), playerGUID(ObjectGuid::Create<HighGuid::Player>(guid))
	{
		parameter1 = 0;
		parameter2 = 0;
		parameter3 = 0;
		parameter4 = 0;
		parameter5 = 0;

		scheduleState = 0;
		processTick = 0;
	}
};

class PlayerBotSession : public WorldSession
{
	typedef std::list<BotGlobleSchedule> BotSchedules;

public:
	PlayerBotSession(uint32 id, std::string &name, AccountTypes sec, uint8 expansion, time_t mute_time, LocaleConstant locale, uint32 recruiter, bool isARecruiter);

	~PlayerBotSession()
	{

	}

	bool IsBotSession() override;
	bool HasBGSchedule() override;
	bool Update(uint32 diff, Map* map = nullptr) override;

	void PushScheduleToQueue(BotGlobleSchedule& schedule);
	void RemoveScheduleByType(BotGlobleScheduleType eType);
	bool HasScheduleByType(BotGlobleScheduleType eType);
	bool HasSchedules() override;
	bool IsAccountBotSession() override;
	void ClearAllSchedule() { m_Schedules.clear(); }
	void SetAccountBotSession() { m_AccountBot = true; }

private:
	bool PlayerIsReady();
	void ProcessNoWorld(uint32 diff);
	void CastSchedule(uint32 diff);
	bool ProcessOnline(BotGlobleSchedule& schedule);
	bool ProcessOnlineByGUID(BotGlobleSchedule& schedule);
	bool ProcessOffline(BotGlobleSchedule& schedule);
	bool ProcessSetting(BotGlobleSchedule& schedule);
	bool ProcessInBGQueue(BotGlobleSchedule& schedule);
	bool ProcessOutBGQueue(BotGlobleSchedule& schedule);
	bool ProcessEnterBG(BotGlobleSchedule& schedule);
	bool ProcessLeaveBG(BotGlobleSchedule& schedule);
	bool ProcessInAAQueue(BotGlobleSchedule& schedule);
	bool ProcessOutAAQueue(BotGlobleSchedule& schedule);
	bool ProcessEnterAA(BotGlobleSchedule& schedule);
	bool ProcessLeaveAA(BotGlobleSchedule& schedule);
	bool ProcessDelayLevelup(BotGlobleSchedule& schedule);
	bool ProcessInLFGQueue(BotGlobleSchedule& schedule);
	bool ProcessOutLFGQueue(BotGlobleSchedule& schedule);
	bool ProcessAcceptLFGProposal(BotGlobleSchedule& schedule);
	bool ProcessOfferPetitionSign(BotGlobleSchedule& schedule);

private:
	BotSchedules m_Schedules;
	int m_LastCastTime;
	int m_NoWorldTick;
	bool m_AccountBot;
	std::mutex m_optQueueLock;
};

#endif //__PlayerBotSESSION_H
