
/// @file GbmCegarOneHot.cc
/// @brief GbmCegarOneHot の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmCegarOneHot.h"
#include "GbmEngineOneHot.h"
#include "ym_logic/SatStats.h"
#include "ym_logic/SatMsgHandlerImpl1.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

const bool debug = false;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス GbmCegarOneHot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GbmCegarOneHot::GbmCegarOneHot()
{
}

// @brief デストラクタ
GbmCegarOneHot::~GbmCegarOneHot()
{
}

// @brief 入力順を考慮したマッチング問題を解く
// @param[in] network RcfNetwork
// @param[in] func マッチング対象の関数
// @param[out] conf_bits configuration ビットの値を収める配列
// @param[out] iorder 入力順序
//             iorder[pos] に network の pos 番めの入力に対応した
//             関数の入力番号が入る．
bool
GbmCegarOneHot::_solve(const RcfNetwork& network,
		       const TvFunc& func,
		       vector<bool>& conf_bits,
		       vector<ymuint>& iorder)
{
#if 1
  SatSolver solver("minisat");
#else
  SatSolver solver;
#endif

  ymuint nc = network.conf_var_num();
  ymuint nn = network.node_num();
  ymuint ni = network.input_num();

  GbmEngineOneHot engine(solver, nn, nc, ni);

  SatMsgHandlerImpl1 satmsghandler(cout);
  solver.reg_msg_handler(&satmsghandler);
  solver.timer_on(true);

  solver.set_max_conflict(100 * 1024);

  conf_bits.resize(nc, false);
  iorder.resize(ni, 0);

  // 外部出力のノード番号と極性
  RcfNodeHandle output = network.output();
  ymuint oid = output.id();
  bool oinv = output.inv();

  ymuint ni_exp = 1U << ni;
  vector<bool> check(ni_exp, false);
  Bool3 stat = kB3X;
  vector<Bool3> model;
  ymuint bit_pat = 0;
  for ( ;; ) {
    check[bit_pat] = true;
    // 入力に定数を割り当てる．
    if ( debug ) {
      cout << "INPUT: ";
      for (ymuint i = 0; i < ni; ++ i) {
	if ( bit_pat & (1U << (ni - i - 1)) ) {
	  cout << "1";
	}
	else {
	  cout << "0";
	}
      }
      cout << endl;
    }
    // 外部入力変数に値を割り当てたときの CNF 式を作る．
    ymuint oval = static_cast<bool>(func.value(bit_pat)) ^ oinv;
    bool ok = engine.make_cnf(network, bit_pat, oid, oval);
    if ( !ok ) {
      break;
    }
    stat = solver.solve(model);
    if ( stat == kB3False ) {
      break;
    }
    if ( stat == kB3X ) {
      break;
    }
    // 現在の model で全部の入力が成り立つか調べてみる．
    engine.get_conf_bits(model, conf_bits);
    engine.get_iorder(model, iorder);
    bool pass = true;
    for (ymuint b = 0; b < ni_exp; ++ b) {
      if ( check[b] ) {
	continue;
      }
      ymuint exp_out = func.value(b);
      vector<bool> ival_list(ni);
      for (ymuint i = 0; i < ni; ++ i) {
	ymuint src_pos = iorder[i];
	bool val = (b & (1U << src_pos)) ? true : false;
	ival_list[i] = val;
      }
      if ( network.simulate(ival_list, conf_bits) != exp_out ) {
	bit_pat = b;
	pass = false;
	break;
      }
    }
    if ( pass ) {
      return true;
    }
  }

  if ( stat == kB3X ) {
    cout << "Aborted" << endl;
  }

  return false;
}

END_NAMESPACE_YM