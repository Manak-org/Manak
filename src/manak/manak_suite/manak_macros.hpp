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
struct Name ## _ ## Library                                                   \
{                                                                             \
  static void Run();                                                          \
  static manak::ManakCase* static_temp;                                       \
};                                                                            \
manak::ManakCase* Name ## _ ## Library::static_temp =                         \
manak::ManakSuite::GetMasterSuite().GetCurrentSuite()->                       \
AddCase(_MANAK_CASE_TIS(Type,                                                 \
                        Name,                                                 \
                        Library,                                              \
                        Name ## _ ## Library::Run,                            \
                        Tol,                                                  \
                        Iter,                                                 \
                        SP));                                                 \
void Name ## _ ## Library::Run()

////////////////////////////////////////////////////////////////////////////////
/// MANAK CREATE BENCHMARK WITH TEMPLATE MACRO
////////////////////////////////////////////////////////////////////////////////

#define _MANAK_CREATE_WITH_TEMPLATE_TIS(Type, Name, Library, Function, Tol, Iter, SP)  \
( manak::CTManakCase<manak::Type>(#Name, MANAK_STRINGIZE(Library), Function, Tol, Iter, SP) )

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
  template<typename... Args>                                                  \
  void InitCaller(Args... args)                                               \
  {                                                                           \
    InitCaller2<decltype(GetType())>(0, args...);                             \
  }                                                                           \
                                                                              \
  template<typename T, typename... Args>                                      \
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
  template<size_t n, typename T>                                              \
  struct AddCase                                                              \
  {                                                                           \
    AddCase()                                                                 \
    {                                                                         \
      AddCase<n + 1, T>();                                                    \
    }                                                                         \
  };


#define MANAK_GROUP_END()                                                     \
  template<typename T>                                                        \
  struct AddCase<__LINE__, T>                                                 \
  {};                                                                         \
};

#define GINIT                                                                 \
template<typename abcde, typename... Args>                                    \
void InitCaller2(abcde*, Args... args) \
{                                                                             \
  static_cast<abcde*>(this)->Manak_Group_Initialize(args...);                 \
};                                                                            \
void Manak_Group_Initialize

#define GDOWN void Manak_Group_TearDown

////////////////////////////////////////////////////////////////////////////////
/// MANAK ADD GROUP MACROS
////////////////////////////////////////////////////////////////////////////////

#define MANAK_AG_GET_MACRO(_1,_2, NAME,...) NAME
#define MANAK_ADD_GROUP(...) MANAK_AG_GET_MACRO(__VA_ARGS__, MANAK_ADD_GROUP_2, MANAK_ADD_GROUP_1)(__VA_ARGS__)

#define MANAK_ADD_GROUP_1(Name)                                               \
struct STRING_JOIN(Manak_unamed, __LINE__ )                                   \
{                                                                             \
  static bool value;                                                          \
};                                                                            \
bool STRING_JOIN(Manak_unamed,__LINE__)::value =                              \
 manak::ManakSuite::GetMasterSuite().GetCurrentSuite()->                      \
    AddGroup(MG ## _ ## Name::Global())

#define MANAK_ADD_GROUP_2(Name, ...)                                               \
struct STRING_JOIN(Manak_unamed, __LINE__ )                                   \
{                                                                             \
  static bool value;                                                          \
};                                                                            \
bool STRING_JOIN(Manak_unamed,__LINE__)::value =                              \
 manak::ManakSuite::GetMasterSuite().GetCurrentSuite()->                      \
    AddGroup(MG ## _ ## Name::Global(), __VA_ARGS__)

////////////////////////////////////////////////////////////////////////////////
/// MANAK GROUP CASE MACROS
////////////////////////////////////////////////////////////////////////////////

#define MANAK_ADD_TO_GROUP(case)                                              \
static void STRING_JOIN(group_caller, __LINE__)()                             \
{                                                                             \
  Global().AddManakCase(case);                                                \
}                                                                             \
template<typename T>                                                          \
struct AddCase<__LINE__, T>                                                   \
{                                                                             \
  AddCase()                                                                   \
  {                                                                           \
    STRING_JOIN(group_caller, __LINE__)();                                    \
    AddCase<__LINE__ + 1, T>();                                               \
  }                                                                           \
}

#define _MANAK_GROUP_CASE_TIS(Type, Name, Library, Function, Tol, Iter, SP)         \
( new manak::Type(#Name, #Library, manak::utils::BindToObject(&decltype(GetType())::Function, &Global()), Tol, Iter, SP) )
