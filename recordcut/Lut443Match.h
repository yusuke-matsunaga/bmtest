﻿#ifndef LUT443MATCH_H
#define LUT443MATCH_H

/// @file Lut443Match.h
/// @brief Lut443Match のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "RcfNetwork.h"
#include "YmLogic/TvFunc.h"
#include "GbmSolver.h"
#include "LnMatch.h"
#include "YmUtils/USTime.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Lut443Match Lut443Match.h "Lut443Match.h"
/// @brief 4-4-3 の LUT ネットワークに対するマッチングを行うクラス
//////////////////////////////////////////////////////////////////////
class Lut443Match
{
public:

  /// @brief コンストラクタ
  Lut443Match();

  /// @brief デストラクタ
  ~Lut443Match();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 与えられた関数を LUT ネットワークで実現できるか調べる．
  /// @param[in] func 対象の関数
  /// @param[in] solver GBM ソルバ
  bool
  match(const TvFunc& func,
	GbmSolver& solver);

  /// @brief カウンタをリセットする．
  void
  reset_count();

  /// @brief カウンタの値を得る．
  void
  get_count(ymuint& trivial_num,
	    ymuint& a0_num,
	    ymuint& a0_loop,
	    ymuint& a1_num,
	    ymuint& a1_loop,
	    ymuint& a2_num,
	    ymuint& a2_loop,
	    ymuint& a3_num,
	    ymuint& a3_loop,
	    ymuint& b0_num,
	    ymuint& b0_loop,
	    ymuint& b1_num,
	    ymuint& b1_loop,
	    ymuint& b2_num,
	    ymuint& b2_loop,
	    ymuint& c0_num,
	    ymuint& c0_loop,
	    ymuint& c1_num,
	    ymuint& c1_loop,
	    ymuint& fail_num,
	    USTime& ok_time,
	    USTime& ng_time);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  void
  make_lut443(ymuint input_num,
	      ymuint iorder[],
	      ymuint sym_list[]);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  LnMatch mMatcher;

  USTime mOKTime;

  USTime mNGTime;

};

END_NAMESPACE_YM

#endif // LUT443MATCH_H
