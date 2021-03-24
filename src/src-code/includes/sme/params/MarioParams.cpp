#include "OS.h"
#include "sms/actor/Mario.hxx"
#include "string.h"

#include "libs/sMath.hxx"
#include "libs/sMemory.hxx"
#include "params/MarioParams.hxx"

using namespace SME::Class;

PlayerParams::PlayerParams(TMario *player, bool isMario) {
  mPlayer = player;
  mPlayerID = 0;
  mIsEMario = !isMario;
  mFluddHistory.mHadFludd = false;
  mFluddHistory.mMainNozzle = TWaterGun::Spray;
  mFluddHistory.mSecondNozzle = TWaterGun::Hover;
  mFluddHistory.mWaterLevel = 0;
  mCurJump = 0;

  loadPrm(reinterpret_cast<TCustomParams &>(
      *JKRFileLoader::getVolume("mario")->getResource("/params.bin")));

  if (mParams->mPlayerHasGlasses.get())
    wearGlass__6TMarioFv(player);

  scalePlayerAttrs(mParams->mSizeMultiplier.get());
}

void PlayerParams::ParamHistory::applyHistoryTo(TMario *player) {
  player->mDeParams = mDeParams;
  player->mBodyAngleFreeParams = mBodyAngleFreeParams;
  player->mBodyAngleWaterGunParams = mBodyAngleWaterGunParams;
  player->mPunchFenceParams = mPunchFenceParams;
  player->mKickRoofParams = mKickRoofParams;
  player->mJumpParams = mJumpParams;
  player->mRunParams = mRunParams;
  player->mSwimParams = mSwimParams;
  player->mHangFenceParams = mHangFenceParams;
  player->mHangRoofParams = mHangRoofParams;
  player->mWireParams = mWireParams;
  player->mPullBGBeakParams = mPullBGBeakParams;
  player->mPullBGTentacleParams = mPullBGTentacleParams;
  player->mPullBGFireWanWanBossTailParams = mPullBGFireWanWanBossTailParams;
  player->mPullFireWanWanTailParams = mPullFireWanWanTailParams;
  player->mHoverParams = mHoverParams;
  player->mDivingParams = mDivingParams;
  player->mYoshiParams = mYoshiParams;
  player->mWaterEffectParams = mWaterEffectParams;
  player->mControllerParams = mControllerParams;
  player->mGraffitoParams = mGraffitoParams;
  player->mDirtyParams = mDirtyParams;
  player->mMotorParams = mMotorParams;
  player->mParticleParams = mParticleParams;
  player->mEffectParams = mEffectParams;
  player->mSlipNormalParams = mSlipNormalParams;
  player->mSlipOilParams = mSlipOilParams;
  player->mSlipAllParams = mSlipAllParams;
  player->mSlipAllSliderParams = mSlipAllSliderParams;
  player->mSlip45Params = mSlip45Params;
  player->mSlipWaterSlopeParams = mSlipWaterSlopeParams;
  player->mSlipWaterGroundParams = mSlipWaterGroundParams;
  player->mSlipYoshiParams = mSlipYoshiParams;
  player->mUpperBodyParams = mUpperBodyParams;
}

void PlayerParams::ParamHistory::recordFrom(TMario *player) {
  mDeParams = player->mDeParams;
  mBodyAngleFreeParams = player->mBodyAngleFreeParams;
  mBodyAngleWaterGunParams = player->mBodyAngleWaterGunParams;
  mPunchFenceParams = player->mPunchFenceParams;
  mKickRoofParams = player->mKickRoofParams;
  mJumpParams = player->mJumpParams;
  mRunParams = player->mRunParams;
  mSwimParams = player->mSwimParams;
  mHangFenceParams = player->mHangFenceParams;
  mHangRoofParams = player->mHangRoofParams;
  mWireParams = player->mWireParams;
  mPullBGBeakParams = player->mPullBGBeakParams;
  mPullBGTentacleParams = player->mPullBGTentacleParams;
  mPullBGFireWanWanBossTailParams = player->mPullBGFireWanWanBossTailParams;
  mPullFireWanWanTailParams = player->mPullFireWanWanTailParams;
  mHoverParams = player->mHoverParams;
  mDivingParams = player->mDivingParams;
  mYoshiParams = player->mYoshiParams;
  mWaterEffectParams = player->mWaterEffectParams;
  mControllerParams = player->mControllerParams;
  mGraffitoParams = player->mGraffitoParams;
  mDirtyParams = player->mDirtyParams;
  mMotorParams = player->mMotorParams;
  mParticleParams = player->mParticleParams;
  mEffectParams = player->mEffectParams;
  mSlipNormalParams = player->mSlipNormalParams;
  mSlipOilParams = player->mSlipOilParams;
  mSlipAllParams = player->mSlipAllParams;
  mSlipAllSliderParams = player->mSlipAllSliderParams;
  mSlip45Params = player->mSlip45Params;
  mSlipWaterSlopeParams = player->mSlipWaterSlopeParams;
  mSlipWaterGroundParams = player->mSlipWaterGroundParams;
  mSlipYoshiParams = player->mSlipYoshiParams;
  mUpperBodyParams = player->mUpperBodyParams;
}

void PlayerParams::ParamHistory::reset() {
  mHasHistory = false;
  memset(this, 0, sizeof(ParamHistory));
}

void PlayerParams::scalePlayerAttrs(f32 scale) {
  const float factor = (scale * 0.5f) + (1.0f - 0.5f);

  JGeometry::TVec3<f32> size;
  mPlayer->JSGGetScaling(reinterpret_cast<Vec *>(&size));
  size.scale(scale);
  mPlayer->JSGSetScaling(reinterpret_cast<Vec &>(size));
  mPlayer->mModelData->mModel->mScale.scale(scale);

  const f32 yoshiAgility =
      SME::Util::Math::sigmoidCurve(size.y, 0.8f, 5.0f, 1.6f, 5.0f);

#define SCALE_PARAM(param, scale) param.set(param.get() * scale)

  SCALE_PARAM(mPlayer->mDeParams.mRunningMax, factor);
  SCALE_PARAM(mPlayer->mDeParams.mDashMax, factor);
  SCALE_PARAM(mPlayer->mDeParams.mShadowSize, scale);
  SCALE_PARAM(mPlayer->mDeParams.mHoldRadius, scale);
  SCALE_PARAM(mPlayer->mDeParams.mDamageRadius, scale);
  SCALE_PARAM(mPlayer->mDeParams.mDamageHeight, scale);
  SCALE_PARAM(mPlayer->mDeParams.mAttackHeight, scale);
  SCALE_PARAM(mPlayer->mDeParams.mTrampleRadius, scale);
  SCALE_PARAM(mPlayer->mDeParams.mPushupRadius, scale);
  SCALE_PARAM(mPlayer->mDeParams.mPushupHeight, scale);
  SCALE_PARAM(mPlayer->mDeParams.mHipdropRadius, scale);
  SCALE_PARAM(mPlayer->mDeParams.mQuakeRadius, scale);
  SCALE_PARAM(mPlayer->mDeParams.mQuakeHeight, scale);
  SCALE_PARAM(mPlayer->mDeParams.mJumpWallHeight, factor);
  SCALE_PARAM(mPlayer->mDeParams.mThrowPower, factor);
  SCALE_PARAM(mPlayer->mDeParams.mFeelDeep, factor);
  SCALE_PARAM(mPlayer->mDeParams.mDamageFallHeight, factor);
  SCALE_PARAM(mPlayer->mDeParams.mClashSpeed, factor);
  SCALE_PARAM(mPlayer->mDeParams.mSleepingCheckDist, factor);
  SCALE_PARAM(mPlayer->mDeParams.mSleepingCheckHeight, factor);
  SCALE_PARAM(mPlayer->mPunchFenceParams.mRadius, factor);
  SCALE_PARAM(mPlayer->mPunchFenceParams.mHeight, factor);
  SCALE_PARAM(mPlayer->mKickRoofParams.mRadius, factor);
  SCALE_PARAM(mPlayer->mKickRoofParams.mHeight, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mGravity, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mSpinJumpGravity, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mJumpingMax, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mFenceSpeed, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mFireBackVelocity, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mBroadJumpForce, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mBroadJumpForceY, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mRotateJumpForceY, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mPopUpSpeedY, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mBackJumpForce, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mBackJumpForceY, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mHipAttackSpeedY, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mSuperHipAttackSpeedY, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mRotBroadJumpForce, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mRotBroadJumpForceY, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mSecJumpForce, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mValleyDepth, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mTremblePower, 1 / factor);
  SCALE_PARAM(mPlayer->mJumpParams.mTrembleTime, static_cast<s16>(1 / factor));
  SCALE_PARAM(mPlayer->mJumpParams.mGetOffYoshiY, factor);
  SCALE_PARAM(mPlayer->mJumpParams.mSuperHipAttackCt,
              static_cast<s16>(1 / factor));
  SCALE_PARAM(mPlayer->mRunParams.mMaxSpeed, factor);
  SCALE_PARAM(mPlayer->mRunParams.mAddBase, factor);
  SCALE_PARAM(mPlayer->mRunParams.mDecBrake, factor);
  SCALE_PARAM(mPlayer->mRunParams.mSoft2Walk, factor);
  SCALE_PARAM(mPlayer->mRunParams.mWalk2Soft, factor);
  SCALE_PARAM(mPlayer->mRunParams.mSoftStepAnmMult, 1 / factor);
  SCALE_PARAM(mPlayer->mRunParams.mRunAnmSpeedMult, 1 / factor);
  SCALE_PARAM(mPlayer->mRunParams.mMotBlendWalkSp, 1 / factor);
  SCALE_PARAM(mPlayer->mRunParams.mMotBlendRunSp, 1 / factor);
  SCALE_PARAM(mPlayer->mRunParams.mSwimDepth, factor);
  SCALE_PARAM(mPlayer->mRunParams.mTurnNeedSp, factor);
  SCALE_PARAM(mPlayer->mSwimParams.mStartSp, factor);
  SCALE_PARAM(mPlayer->mSwimParams.mMoveSp, factor);
  SCALE_PARAM(mPlayer->mSwimParams.mGravity, factor);
  SCALE_PARAM(mPlayer->mSwimParams.mWaitBouyancy, factor);
  SCALE_PARAM(mPlayer->mSwimParams.mMoveBouyancy, factor);
  SCALE_PARAM(mPlayer->mSwimParams.mCanJumpDepth, scale);
  SCALE_PARAM(mPlayer->mSwimParams.mEndDepth, scale);
  SCALE_PARAM(mPlayer->mSwimParams.mFloatHeight, scale);
  SCALE_PARAM(mPlayer->mSwimParams.mRush, factor);
  SCALE_PARAM(mPlayer->mSwimParams.mPaddleSpeedUp, factor);
  SCALE_PARAM(mPlayer->mSwimParams.mPaddleJumpUp, factor);
  SCALE_PARAM(mPlayer->mSwimParams.mFloatUp, factor);
  SCALE_PARAM(mPlayer->mSwimParams.mPaddleDown, factor);
  SCALE_PARAM(mPlayer->mSwimParams.mCanBreathDepth, scale);
  SCALE_PARAM(mPlayer->mSwimParams.mWaitSinkSpeed, factor);
  SCALE_PARAM(mPlayer->mHangFenceParams.mMoveSp, factor);
  SCALE_PARAM(mPlayer->mHangFenceParams.mDescentSp, factor);
  SCALE_PARAM(mPlayer->mPullBGBeakParams.mPullRateV, factor);
  SCALE_PARAM(mPlayer->mPullBGBeakParams.mPullRateH, factor);
  SCALE_PARAM(mPlayer->mPullBGBeakParams.mOilPullRateV, factor);
  SCALE_PARAM(mPlayer->mPullBGBeakParams.mOilPullRateH, factor);
  SCALE_PARAM(mPlayer->mPullBGTentacleParams.mPullRateV, factor);
  SCALE_PARAM(mPlayer->mPullBGTentacleParams.mPullRateH, factor);
  SCALE_PARAM(mPlayer->mPullBGTentacleParams.mOilPullRateV, factor);
  SCALE_PARAM(mPlayer->mPullBGTentacleParams.mOilPullRateH, factor);
  SCALE_PARAM(mPlayer->mPullBGFireWanWanBossTailParams.mPullRateV, factor);
  SCALE_PARAM(mPlayer->mPullBGFireWanWanBossTailParams.mPullRateH, factor);
  SCALE_PARAM(mPlayer->mPullBGFireWanWanBossTailParams.mOilPullRateV, factor);
  SCALE_PARAM(mPlayer->mPullBGFireWanWanBossTailParams.mOilPullRateH, factor);
  SCALE_PARAM(mPlayer->mPullFireWanWanTailParams.mPullRateV, factor);
  SCALE_PARAM(mPlayer->mPullFireWanWanTailParams.mPullRateH, factor);
  SCALE_PARAM(mPlayer->mPullFireWanWanTailParams.mOilPullRateV, factor);
  SCALE_PARAM(mPlayer->mPullFireWanWanTailParams.mOilPullRateH, factor);
  SCALE_PARAM(mPlayer->mDivingParams.mGravity, factor);
  SCALE_PARAM(mPlayer->mYoshiParams.mRunYoshiMult, yoshiAgility);
  SCALE_PARAM(mPlayer->mYoshiParams.mJumpYoshiMult, yoshiAgility);
  SCALE_PARAM(mPlayer->mYoshiParams.mRotYoshiMult, yoshiAgility);
  SCALE_PARAM(mPlayer->mWaterEffectParams.mJumpIntoMinY, factor);
  SCALE_PARAM(mPlayer->mWaterEffectParams.mJumpIntoMaxY, factor);
  SCALE_PARAM(mPlayer->mWaterEffectParams.mJumpIntoScaleMin, factor);
  SCALE_PARAM(mPlayer->mWaterEffectParams.mJumpIntoScaleWidth, factor);
  SCALE_PARAM(mPlayer->mWaterEffectParams.mRunningRippleDepth, factor);
  SCALE_PARAM(mPlayer->mGraffitoParams.mSinkHeight, factor);
  SCALE_PARAM(mPlayer->mGraffitoParams.mSinkMoveMin, factor);
  SCALE_PARAM(mPlayer->mGraffitoParams.mSinkMoveMax, factor);
  SCALE_PARAM(mPlayer->mGraffitoParams.mSinkRecover, factor);
  SCALE_PARAM(mPlayer->mGraffitoParams.mFootEraseSize, scale);
  SCALE_PARAM(mPlayer->mDirtyParams.mPolSizeSlip, scale);
  SCALE_PARAM(mPlayer->mDirtyParams.mPolSizeRun, scale);
  SCALE_PARAM(mPlayer->mDirtyParams.mPolSizeFootPrint, scale);
  SCALE_PARAM(mPlayer->mDirtyParams.mPolSizeJump, scale);
  SCALE_PARAM(mPlayer->mDirtyParams.mSlipAnmSpeed, 1 / factor);
  SCALE_PARAM(mPlayer->mDirtyParams.mSlipRunSp, factor);
  SCALE_PARAM(mPlayer->mDirtyParams.mSlipCatchSp, factor);

#undef SCALE_PARAM
}

void PlayerParams::setPlayer(TMario *player) {
  mPlayer = player;
  mDefaultAttrs.recordFrom(player);
}

bool PlayerParams::loadPrm() {
  mParams->load("/Mario/SME.prm");
  mInitialized = true;
  return true;
}

bool PlayerParams::loadPrm(const char *prm) {
  mParams->load(prm);
  mInitialized = true;
  return true;
}

bool PlayerParams::loadPrm(TCustomParams &data) {
  memcpy(mParams, &data, sizeof(TCustomParams));

  mInitialized = true;
  return true;
}

bool PlayerParams::canUseNozzle(TWaterGun::NozzleType nozzle) const {
  switch (nozzle) {
  case TWaterGun::NozzleType::Spray:
    return mParams->mSprayNozzleUsable.get();
  case TWaterGun::NozzleType::Rocket:
    return mParams->mRocketNozzleUsable.get();
  case TWaterGun::NozzleType::Underwater:
    return mParams->mUnderwaterNozzleUsable.get();
  case TWaterGun::NozzleType::Yoshi:
    return mParams->mYoshiNozzleUsable.get();
  case TWaterGun::NozzleType::Hover:
    return mParams->mHoverNozzleUsable.get();
  case TWaterGun::NozzleType::Turbo:
    return mParams->mTurboNozzleUsable.get();
  case TWaterGun::NozzleType::Sniper:
    return mParams->mSniperNozzleUsable.get();
  default:
    return false;
  }
}
