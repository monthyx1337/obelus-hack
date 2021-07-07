#pragma once
#define CheckIfNonValidNumber(x) (fpclassify(x) == FP_INFINITE || fpclassify(x) == FP_NAN || fpclassify(x) == FP_SUBNORMAL)

namespace misc
{
	void Bunnyhop();

	void InfiniteDuck();

	void JumpBug(int, int);
	void EdgeJump(int);
	void AutoStrafe();
	void RecoilCrosshair();
	void RankRevealer();
	void ViewModelOffset();
	void FastStop();
	void DisableInterpolation();

	inline bool m_bIsJumpbugging;
	inline bool m_bIsEdgebugging;
}