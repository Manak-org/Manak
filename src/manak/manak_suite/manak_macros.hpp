////////////////////////////////////////////////////////////////////////////////
/// MANAK SUITE MACROS
////////////////////////////////////////////////////////////////////////////////

#define MANAK_SUITE(Name)                                                     \
( new manak::ManakSuite(#Name) )

#define MANAK_AUTO_SUITE(Name)                                                \
static manak::RegisterManakSuite STRING_JOIN(X, STRING_JOIN(invoker, __LINE__))(#Name)  \

#define MANAK_AUTO_TEST(Name, X) static const size_t Name = 0

#define MANAK_AUTO_SUITE_END()                                                \
static manak::DeRegisterManakSuite STRING_JOIN(destroy, __LINE__)

////////////////////////////////////////////////////////////////////////////////
/// MANAK CASE MACROS
////////////////////////////////////////////////////////////////////////////////

#define MANAK_ADD_CASE(bench)                                                 \
class STRING_JOIN(unamed, STRING_JOIN(_, __LINE__))                           \
{                                                                             \
  static manak::ManakCase* static_temp;                                       \
};                                                                            \
manak::ManakCase*                                                             \
STRING_JOIN(unamed, STRING_JOIN(_, __LINE__))::static_temp =                  \
manak::ManakSuite::GetMasterSuite().GetCurrentSuite()->AddCase(bench)

#define _MANAK_CASE_TIS(Type, Name, Library, Function, Tol, Iter, SP)         \
( new manak::Type(#Name, #Library, Function, Tol, Iter, SP) )

////////////////////////////////////////////////////////////////////////////////
/// MANAK AUTO CASE MACRO
////////////////////////////////////////////////////////////////////////////////

#define _MANAK_AUTO_CASE_TIS(Type, Name, Library, Tol, Iter, SP)              \
struct STRING_JOIN(Name ## _ ## Library, __LINE__)                            \
{                                                                             \
  static void Run();                                                          \
  static manak::ManakCase* static_temp;                                       \
};                                                                            \
manak::ManakCase* STRING_JOIN(Name ## _ ## Library, __LINE__)::static_temp =  \
manak::ManakSuite::GetMasterSuite().GetCurrentSuite()->                       \
AddCase(_MANAK_CASE_TIS(Type,                                                 \
                        Name,                                                 \
                        Library,                                              \
                        STRING_JOIN(Name ## _ ## Library, __LINE__)::Run,     \
                        Tol,                                                  \
                        Iter,                                                 \
                        SP));                                                 \
void STRING_JOIN(Name ## _ ## Library, __LINE__)::Run()

////////////////////////////////////////////////////////////////////////////////
/// MANAK CREATE BENCHMARK WITH TEMPLATE MACRO
////////////////////////////////////////////////////////////////////////////////

#define _MANAK_CREATE_WITH_TEMPLATE_TIS(Type, Name, Library, Function, Tol, Iter, SP)  \
( manak::CTManakCase<manak::Type>(#Name, #Library, Function, Tol, Iter, SP) )

////////////////////////////////////////////////////////////////////////////////
/// MANAK GROUP MACROS
////////////////////////////////////////////////////////////////////////////////

#define MANAK_GROUP(Name)                                                     \
class MG ## _ ## Name                                                         \
{                                                                             \
 public:                                                                      \
  MG ## _ ## Name()                                                           \
  {}                                                                          \
                                                                              \
  ~MG ## _ ## Name()                                                          \
  {                                                                           \
    TDCaller<decltype(GetType())>(0);                                         \
  }                                                                           \
                                                                              \
  template<typename... ABCDE>                                                 \
  void InitCaller(ABCDE... abcde)                                             \
  {                                                                           \
    InitCaller2<decltype(GetType())>(0, abcde...);                            \
  }                                                                           \
                                                                              \
  template<typename, typename...>                                             \
  void InitCaller2(...)                                                       \
  {}                                                                          \
                                                                              \
  template<typename abcde>                                                    \
  void TDCaller(manak::utils::manak_group_test<abcde, &abcde::Manak_Group_TearDown>*) \
  {                                                                           \
    static_cast<abcde*>(this)->Manak_Group_TearDown();                        \
  }                                                                           \
  template<typename>                                                          \
  void TDCaller(...)                                                          \
  {}                                                                          \
                                                                              \
  bool AddManakCase(manak::ManakCase* mc)                                     \
  {                                                                           \
    cases.push_back(mc);                                                      \
    return true;                                                              \
  }                                                                           \
                                                                              \
  std::list<manak::ManakCase*> cases;                                         \
                                                                              \
  static MG ## _ ## Name& Global()                                            \
  {                                                                           \
    static MG ## _ ## Name singleton;                                         \
    return singleton;                                                         \
  }                                                                           \
  static MG ## _ ## Name GetType()                                            \
  {                                                                           \
    MG ## _ ## Name t;                                                        \
    return t;                                                                 \
  };                                                                          \
                                                                              \
  void ACCaller()                                                             \
  {                                                                           \
    AddCase<__LINE__, size_t> a;                                              \
  }                                                                           \
                                                                              \
  template<size_t ABCDE, typename ABCDEF>                                     \
  struct AddCase                                                              \
  {                                                                           \
    AddCase()                                                                 \
    {                                                                         \
      AddCase<ABCDE + 1, ABCDEF>();                                           \
    }                                                                         \
  };


#define MANAK_GROUP_END()                                                     \
  template<typename ABCDE>                                                    \
  struct AddCase<__LINE__, ABCDE>                                             \
  {};                                                                         \
};

#define GINIT                                                                 \
template<typename abcde, typename... ABCDEF>                                  \
void InitCaller2(abcde*, ABCDEF... args)                                      \
{                                                                             \
  static_cast<abcde*>(this)->Manak_Group_Initialize(args...);                 \
};                                                                            \
void Manak_Group_Initialize

#define GDOWN void Manak_Group_TearDown

////////////////////////////////////////////////////////////////////////////////
/// MANAK AUTO GROUP MACROS
////////////////////////////////////////////////////////////////////////////////

#define F_MANAK_ADD_GROUP(X) MANAK_ADD_GROUP(X)
#define F_MANAK_GROUP(X) MANAK_GROUP(X)

#define MANAK_AUTO_GROUP()                                                    \
class STRING_JOIN(MG_unamed, __LINE__);                                       \
F_MANAK_ADD_GROUP(STRING_JOIN(unamed, __LINE__));                             \
F_MANAK_GROUP(STRING_JOIN(unamed, __LINE__));

#define MANAK_AUTO_GROUP_END()                                                \
MANAK_GROUP_END();

////////////////////////////////////////////////////////////////////////////////
/// MANAK ADD GROUP MACROS
////////////////////////////////////////////////////////////////////////////////

#define MANAK_ADD_GROUP(Name, ...)                                            \
struct STRING_JOIN(Manak_unamed, __LINE__ )                                   \
{                                                                             \
  static bool value;                                                          \
};                                                                            \
bool STRING_JOIN(Manak_unamed,__LINE__)::value =                              \
 manak::ManakSuite::GetMasterSuite().GetCurrentSuite()->                      \
    AddGroup<MG ## _ ## Name>(__VA_ARGS__)

////////////////////////////////////////////////////////////////////////////////
/// MANAK GROUP CASE MACROS
////////////////////////////////////////////////////////////////////////////////

#define MANAK_ADD_TO_GROUP(case)                                              \
void STRING_JOIN(group_caller, __LINE__)()                                    \
{                                                                             \
  Global().AddManakCase(case);                                                \
}                                                                             \
template<typename T>                                                          \
struct AddCase<__LINE__, T>                                                   \
{                                                                             \
  AddCase()                                                                   \
  {                                                                           \
    manak::utils::BindToObject(&decltype(GetType())::STRING_JOIN(group_caller, __LINE__), &Global())();                                    \
    AddCase<__LINE__ + 1, T>();                                               \
  }                                                                           \
}

#define _MANAK_GROUP_CASE_TIS(Type, Name, Library, Function, Tol, Iter, SP)   \
( new manak::Type(Name, Library, manak::utils::BindToObject(&decltype(GetType())::Function, &Global()), Tol, Iter, SP) )

////////////////////////////////////////////////////////////////////////////////
/// MANAK AUTO GROUP CASE
////////////////////////////////////////////////////////////////////////////////

#define _MANAK_AUTO_GROUP_CASE_TIS(Type, Name, Lib, Tol, Iter, SP)            \
MANAK_ADD_TO_GROUP(_MANAK_GROUP_CASE_TIS(Type, Name, Lib, STRING_JOIN(manak_auto_fun, __LINE__), Tol, Iter, SP));  \
void STRING_JOIN(manak_auto_fun, __LINE__)()

////////////////////////////////////////////////////////////////////////////////
/// MANAK CREATE GROUP WITH TEMPLATE MACROS
////////////////////////////////////////////////////////////////////////////////

#define _MANAK_CREATE_GROUP_WITH_TEMPLATE_TIS(Type, Name, Lib, Fun, Tol, Iter, SP)  \
( manak::CTManakCase<manak::Type>(Name, Lib, manak::utils::BindToObject(&decltype(GetType())::Fun, &Global()), Tol, Iter, SP) )

