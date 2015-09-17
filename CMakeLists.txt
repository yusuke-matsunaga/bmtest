# ===================================================================
# CMAKE のおまじない
# ===================================================================
cmake_minimum_required (VERSION 3.0)


# C++11 の指定
set (CMAKE_CXX_STANDARD 11)


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================
project (bmtest)
set (BMTEST_VERSION_MAJOR 1)
set (BMTEST_VERSION_MINOR 0)
set (BMTEST_VERSION ${BMTEST_VERSION_MAJOR}.${BMTEST_VERSION_MINOR})


# ===================================================================
# オプション
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================
list ( APPEND CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/ym-common/cmake"
  )

include (FindDoxygen)

#include (FindPythonInterp)
#include (FindPythonLibs)

find_package(YmTools REQUIRED)

include (FindPOPT)

#find_package(TCL)
#if (TCL_FOUND)
#  find_package(YmTclpp)
#endif (TCL_FOUND)

find_package(Gperftools)

#include (UseCython)
include (ym_add_library)


# ===================================================================
# google-test は内蔵のものを使う．
# ===================================================================
include ( ym_use_gtest )
ym_use_gtest ()


# ===================================================================
# CTest モジュールの読み込み
# ===================================================================
include (CTest)

enable_testing()


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  ${YmTools_INCLUDE_DIRS}
  ${POPT_INCLUDE_DIR}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

#if (YmTclpp_FOUND)
#  add_subdirectory (atpg_tclsh)
#endif (YmTclpp_FOUND)

#if (CYTHON_FOUND)
#  add_subdirectory (py-satpg)
#endif (CYTHON_FOUND)


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (libec_SOURCES
  libec/BottomUp.cc
  libec/EnumCut.cc
  libec/EnumCut2.cc
  libec/TopDown.cc
  libec/TopDown2.cc
  )

set (libgbm_SOURCES
  libgbm/GbmSatCegarBinary.cc
  libgbm/GbmSatCegarEnum.cc
  libgbm/GbmSatCegarOneHot.cc
  libgbm/GbmSatEngine.cc
  libgbm/GbmSatEngineBinary.cc
  libgbm/GbmSatEngineEnum.cc
  libgbm/GbmSatEngineOneHot.cc
  libgbm/GbmSatNaiveBinary.cc
  libgbm/GbmSatNaiveEnum.cc
  libgbm/GbmSatNaiveOneHot.cc
  libgbm/GbmSolver.cc
  libgbm/RcfNetwork.cc
  libgbm/RcfNodeImpl.cc
  )

set (recordcut_SOURCES
  recordcut/recordcut.cc
  recordcut/FuncMgr.cc
  recordcut/FuncRec.cc
  recordcut/Lut443Match.cc
  recordcut/Lut444Match.cc
  recordcut/LnMatch.cc
  )


# ===================================================================
# ターゲットの設定
# ===================================================================
add_executable ( bmtest
  ${libec_SOURCES}
  ${libgbm_SOURCES}
  ${recordcut_SOURCES}
  )

target_compile_options ( bmtest
  PRIVATE "-O3"
  )

target_compile_definitions ( bmtest
  PRIVATE "-DNODEBUG"
  )

target_link_libraries ( bmtest
  ym_networks
  ${POPT_LIBRARY}
  )

add_executable ( bmtest_p
  ${libec_SOURCES}
  ${libgbm_SOURCES}
  ${recordcut_SOURCES}
  )

target_compile_options ( bmtest_p
  PRIVATE "-O2" "-g"
  )

target_compile_definitions ( bmtest_p
  PRIVATE "-DNODEBUG"
  )

target_link_libraries ( bmtest_p
  ym_networks_p
  ${POPT_LIBRARY}
  )

add_executable ( bmtest_d
  ${libec_SOURCES}
  ${libgbm_SOURCES}
  ${recordcut_SOURCES}
  )

target_compile_options ( bmtest_d
  PRIVATE "-g"
  )

target_link_libraries ( bmtest_d
  ym_networks_d
  ${POPT_LIBRARY}
  )


# ===================================================================
#  テストの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
