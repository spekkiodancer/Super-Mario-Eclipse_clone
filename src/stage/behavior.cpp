#include <JSystem/JDrama/JDRNameRef.hxx>
#include <JSystem/JParticle/JPAResourceManager.hxx>

#include <SMS/MSound/MSound.hxx>
#include <SMS/MSound/MSoundSESystem.hxx>
#include <SMS/Manager/FlagManager.hxx>
#include <SMS/raw_fn.hxx>

#include <BetterSMS/libs/boundbox.hxx>
#include <BetterSMS/module.hxx>
#include <BetterSMS/objects/generic.hxx>

#include "settings.hxx"
#include "stage.hxx"

#pragma region CruiserDelfino

static bool s_cruiser_checked = false;

BETTER_SMS_FOR_CALLBACK void resetCruiserUnlocked(TMarDirector *director) {
    s_cruiser_checked = false;
}

BETTER_SMS_FOR_CALLBACK void checkForCruiserUnlocked(TMarDirector *director) {
    if (s_cruiser_checked)
        return;

    s_cruiser_checked = true;

    auto *nameRef = TMarNameRefGen::getInstance()->getRootNameRef();
    u16 keyCode   = JDrama::TNameRef::calcKeyCode("mini_cruiser");
    if (JDrama::TNameRef *p = nameRef->searchF(keyCode, "mini_cruiser")) {
        TGenericRailObj *cruiser = reinterpret_cast<TGenericRailObj *>(p);

        bool cruiser_unlocked =
            TFlagManager::smInstance->getBool(0x10077);  // Corona Mountain beaten

        if (cruiser_unlocked) {
            cruiser->makeObjAppeared();
        } else {
            cruiser->makeObjDead();
        }
    }
}

#pragma region SnowParticles

static void checkIceStageSweat() {
    TMario *player;
    SMS_FROM_GPR(31, player);

    if (gpMarDirector->mAreaID == 70)
        return;

    *reinterpret_cast<u32 *>(&player->mAttributes) |= 0x20;
}
SMS_PATCH_BL(SMS_PORT_REGION(0x8024FC58, 0, 0, 0), checkIceStageSweat);

static void loadIceStageWalkSandA(JPAResourceManager *manager, const char *old_res, u16 id) {
    if (gpMarDirector->mAreaID == 70 || gpMarDirector->mAreaID == 80 ||
        gpMarDirector->mAreaID == 86)
        manager->load("ms_m_walksnow_a.jpa", id);
    else
        manager->load(old_res, id);
}
SMS_PATCH_BL(0x802B3ED0, loadIceStageWalkSandA);

static void loadIceStageWalkSandB(JPAResourceManager *manager, const char *old_res, u16 id) {
    if (gpMarDirector->mAreaID == 70 || gpMarDirector->mAreaID == 80 ||
        gpMarDirector->mAreaID == 86)
        manager->load("ms_m_walksnow_b.jpa", id);
    else
        manager->load(old_res, id);
}
SMS_PATCH_BL(0x802B3EF8, loadIceStageWalkSandA);

static void loadIceStageSlideSandA(JPAResourceManager *manager, const char *old_res, u16 id) {
    if (gpMarDirector->mAreaID == 70 || gpMarDirector->mAreaID == 80 ||
        gpMarDirector->mAreaID == 86)
        manager->load("ms_m_slidesnow_a.jpa", id);
    else
        manager->load(old_res, id);
}
SMS_PATCH_BL(0x802B4448, loadIceStageWalkSandA);

static void loadIceStageSlideSandB(JPAResourceManager *manager, const char *old_res, u16 id) {
    if (gpMarDirector->mAreaID == 70 || gpMarDirector->mAreaID == 80 ||
        gpMarDirector->mAreaID == 86)
        manager->load("ms_m_slidesnow_b.jpa", id);
    else
        manager->load(old_res, id);
}
SMS_PATCH_BL(0x802B4470, loadIceStageWalkSandA);

#pragma endregion

#pragma region OutsideDelfinoBehavior

BETTER_SMS_FOR_CALLBACK void updateWarpStatesForCruiserCabin(TMarDirector *director) {
    if (director->mAreaID != SME::STAGE_CRUISER) {
        return;
    }

    size_t shine_count = TFlagManager::smInstance->getFlag(0x40000);

    J3DGXColor enabled_color  = {0, 255, 0, 255};
    J3DGXColor disabled_color = {200, 50, 50, 255};
    J3DGXColor off_color      = {0, 0, 0, 255};
    J3DGXColor on_color       = {255, 255, 255, 255};

    JDrama::TNameRef *name_ref = TMarNameRefGen::getInstance()->getRootNameRef();

    {
        bool is_enabled = shine_count >= 120;

        u16 key_code = JDrama::TNameRef::calcKeyCode("LanciaLight");
        if (JDrama::TNameRef *p = name_ref->searchF(key_code, "LanciaLight")) {
            TGenericRailObj *light_obj = reinterpret_cast<TGenericRailObj *>(p);
            J3DTevStage *tev_stage     = light_obj->mActorData->mModel->mModelData->mStages[0];
            J3DTevBlock *tev_block     = *(J3DTevBlock **)((u8 *)tev_stage + 0x28);
            tev_block->setTevKColor(0, is_enabled ? enabled_color : disabled_color);

            if (is_enabled &&
                PSVECDistance(gpMarioAddress->mTranslation, light_obj->mTranslation) < 250.0f) {
                director->setNextStage(SME::STAGE_LANCIA, nullptr);
            }
        }
    }
    {
        bool is_enabled = shine_count >= 120;

        u16 key_code = JDrama::TNameRef::calcKeyCode("VaporLight");
        if (JDrama::TNameRef *p = name_ref->searchF(key_code, "VaporLight")) {
            TGenericRailObj *light_obj = reinterpret_cast<TGenericRailObj *>(p);
            J3DTevStage *tev_stage     = light_obj->mActorData->mModel->mModelData->mStages[0];
            J3DTevBlock *tev_block     = *(J3DTevBlock **)((u8 *)tev_stage + 0x28);
            tev_block->setTevKColor(0, is_enabled ? on_color : off_color);

            if (is_enabled &&
                PSVECDistance(gpMarioAddress->mTranslation, light_obj->mTranslation) < 200.0f) {
                director->setNextStage(SME::STAGE_VAPORWAVE, nullptr);
            }
        }
    }

    {
        bool is_enabled = shine_count >= 140;

        u16 key_code = JDrama::TNameRef::calcKeyCode("CityLight");
        if (JDrama::TNameRef *p = name_ref->searchF(key_code, "CityLight")) {
            TGenericRailObj *light_obj = reinterpret_cast<TGenericRailObj *>(p);
            J3DTevStage *tev_stage     = light_obj->mActorData->mModel->mModelData->mStages[0];
            J3DTevBlock *tev_block     = *(J3DTevBlock **)((u8 *)tev_stage + 0x28);
            tev_block->setTevKColor(0, is_enabled ? enabled_color : disabled_color);

            if (is_enabled &&
                PSVECDistance(gpMarioAddress->mTranslation, light_obj->mTranslation) < 250.0f) {
                director->setNextStage(SME::STAGE_RED_LILY, nullptr);
            }
        }
    }

    {
        bool is_enabled = shine_count >= 160;

        u16 key_code = JDrama::TNameRef::calcKeyCode("YoshiLight");
        if (JDrama::TNameRef *p = name_ref->searchF(key_code, "YoshiLight")) {
            TGenericRailObj *light_obj = reinterpret_cast<TGenericRailObj *>(p);
            J3DTevStage *tev_stage     = light_obj->mActorData->mModel->mModelData->mStages[0];
            J3DTevBlock *tev_block     = *(J3DTevBlock **)((u8 *)tev_stage + 0x28);
            tev_block->setTevKColor(0, is_enabled ? enabled_color : disabled_color);

            if (is_enabled &&
                PSVECDistance(gpMarioAddress->mTranslation, light_obj->mTranslation) < 250.0f) {
                director->setNextStage(SME::STAGE_YOSHI_VILLAGE, nullptr);
            }
        }
    }

    {
        bool is_enabled = shine_count >= 160;

        u16 key_code = JDrama::TNameRef::calcKeyCode("PeachLight");
        if (JDrama::TNameRef *p = name_ref->searchF(key_code, "PeachLight")) {
            TGenericRailObj *light_obj = reinterpret_cast<TGenericRailObj *>(p);
            J3DTevStage *tev_stage     = light_obj->mActorData->mModel->mModelData->mStages[0];
            J3DTevBlock *tev_block     = *(J3DTevBlock **)((u8 *)tev_stage + 0x28);
            tev_block->setTevKColor(0, is_enabled ? enabled_color : disabled_color);

            if (is_enabled &&
                PSVECDistance(gpMarioAddress->mTranslation, light_obj->mTranslation) < 250.0f) {
                director->setNextStage(SME::STAGE_PEACH_BEACH, nullptr);
            }
        }
    }
}

// This allows the redcoinswitch to be immediately pressed in extended stages
// and for the player to have Fludd, while still being an ex stage.
static u8 disableExBehaviorForCruiserWorlds() {
    if (gpApplication.mCurrentScene.mAreaID > 0x54) {
        return 0xFF;
    }
    return SMS_getShineIDofExStage__FUc(gpApplication.mCurrentScene.mAreaID);
}
SMS_PATCH_BL(0x801C08EC, disableExBehaviorForCruiserWorlds);
SMS_PATCH_BL(0x80298B64, disableExBehaviorForCruiserWorlds);
SMS_PATCH_BL(0x802A681C, disableExBehaviorForCruiserWorlds);

#pragma endregion

// Called when a fire is extinguished
static MSound *checkForFireIncrement() {
    TFlagManager::smInstance->incFlag(0x60020, 1);
    return gpMSound;
}
SMS_PATCH_BL(0x80037F04, checkForFireIncrement);

static void checkForSamboFlowerIncrement(MActor *actor, const char *hit_anm_name) {
    actor->setBck(hit_anm_name);
    TFlagManager::smInstance->incFlag(0x60021, 1);
}
SMS_PATCH_BL(0x800E4174, checkForSamboFlowerIncrement);

static void checkForSamboFlowerDecrement(MActor *actor, const char *hit_anm_name) {
    actor->checkCurAnm(hit_anm_name, MActor::BCK);
    TFlagManager::smInstance->incFlag(0x60021, -1);
}
SMS_PATCH_BL(0x800E3F9C, checkForSamboFlowerDecrement);

static void checkHinoKuri2Level(void *hinokuri, int level) {
    if (gpMarDirector->mAreaID == SME::STAGE_LACRIMA_BACKHOUSE) {
        setLevel__10THinokuri2Fi(hinokuri, 2);
    } else {
        setLevel__10THinokuri2Fi(hinokuri, level);
    }
}
SMS_PATCH_BL(0x8005d5e4, checkHinoKuri2Level);
SMS_WRITE_32(0x8005b54c, 0x60000000);

#pragma region 2DWorldBehavior

static BoundingBox s_begin_bound_box = {
    {-14900, 24000, 0   },
    {6000,   16000, 4000},
    {0,      0,     0   }
};

static BoundingBox s_secret_a_bound_box = {
    {14900, 21300, 200 },
    {10000, 11000, 4000},
    {0,     0,     0   }
};

BETTER_SMS_FOR_CALLBACK void forcePlayerZOn2D(TMario *player, bool isMario) {
    if (gpMarDirector->mAreaID != 88) {
        return;
    }

    if (s_secret_a_bound_box.contains(player->mTranslation)) {
        player->mTranslation.z = clamp<f32>(player->mTranslation.z, 150.0f, 250.0f);
        return;
    }

    if (!s_begin_bound_box.contains(player->mTranslation)) {
        player->mTranslation.z = clamp<f32>(player->mTranslation.z, -50.0f, 50.0f);
        return;
    }
}

#pragma endregion

BETTER_SMS_FOR_CALLBACK void resetCoinsOnUniqueStage(TMarDirector *director) {
    if (SMS_getShineStage__FUc(gpApplication.mPrevScene.mAreaID) !=
        SMS_getShineStage__FUc(gpApplication.mCurrentScene.mAreaID)) {
        TFlagManager::smInstance->setFlag(0x40002, 0);
    }
}

// Disable guide menu
SMS_WRITE_32(SMS_PORT_REGION(0x80297A64, 0, 0, 0), 0x4800000C);

// Disable fludd balloons
// SMS_WRITE_32(0x8014A1EC, 0x4E800020);