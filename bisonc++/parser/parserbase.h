// Generated by Bisonc++ V5.02.00-pre on Sun, 15 May 2016 14:32:48 +0530

#ifndef ParserBase_h_included
#define ParserBase_h_included

#include <exception>
#include <vector>
#include <iostream>
// $insert polyincludes
#include <memory>
// $insert preincludes
#include "preheaders.h"
// $insert debugincludes
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <unordered_map>

namespace // anonymous
{
    struct PI__;
}


// $insert polymorphic
enum class Tag__
{
    SYMBOL,
    TERMINAL,
    BOOL,
    SIZE_T,
    BLOCK,
    TEXT,
};

namespace Meta__
{

template <Tag__ tag>
struct TypeOf;

template <typename Tp_>
struct TagOf;

// $insert polymorphicSpecializations
enum { sizeofTag__ = 6 };

extern char const *idOfTag__[];
template <>
struct TagOf<Symbol *>
{
    static Tag__ const tag = Tag__::SYMBOL;
};

template <>
struct TagOf<Terminal *>
{
    static Tag__ const tag = Tag__::TERMINAL;
};

template <>
struct TagOf<bool>
{
    static Tag__ const tag = Tag__::BOOL;
};

template <>
struct TagOf<size_t>
{
    static Tag__ const tag = Tag__::SIZE_T;
};

template <>
struct TagOf<Block>
{
    static Tag__ const tag = Tag__::BLOCK;
};

template <>
struct TagOf<std::string>
{
    static Tag__ const tag = Tag__::TEXT;
};

template <>
struct TypeOf<Tag__::SYMBOL>
{
    typedef Symbol * type;
};

template <>
struct TypeOf<Tag__::TERMINAL>
{
    typedef Terminal * type;
};

template <>
struct TypeOf<Tag__::BOOL>
{
    typedef bool type;
};

template <>
struct TypeOf<Tag__::SIZE_T>
{
    typedef size_t type;
};

template <>
struct TypeOf<Tag__::BLOCK>
{
    typedef Block type;
};

template <>
struct TypeOf<Tag__::TEXT>
{
    typedef std::string type;
};


    // Individual semantic value classes are derived from Base, offering a
    // member returning the value's Tag__, a member cloning the object of its
    // derived Semantic<Tag__> and a member returning a pointerr to its
    // derived Semantic<Tag__> data. See also Bisonc++'s distribution file
    // README.polymorphic-techical
class Base
{
    protected:
        Tag__ d_baseTag;        // d_baseTag is assigned by Semantic.

    public:
        Base() = default;
        Base(Base const &other) = delete;

        virtual ~Base();

        Tag__ tag() const;
        Base *clone() const;
        void *data() const;        

    private:
        virtual Base *vClone() const = 0;
        virtual void *vData() const = 0;
};

inline Base *Base::clone() const
{
    return vClone();
}

inline void *Base::data() const
{
    return vData();
}

inline Tag__ Base::tag() const
{
    return d_baseTag;
}

    // The class Semantic stores a semantic value of the type matching tg_
template <Tag__ tg_>
class Semantic: public Base
{
    typename TypeOf<tg_>::type d_data;
    
    public:
        Semantic();
        Semantic(Semantic<tg_> const &other);   // req'd for cloning

            // This constructor member template forwards its arguments to
            // d_data, allowing it to be initialized using whatever
            // constructor is available for DataType
        template <typename ...Params>
        Semantic(Params &&...params);

    private:
        Base *vClone() const override;
        void *vData() const override;
};

template <Tag__ tg_>
Semantic<tg_>::Semantic()
{
    d_baseTag = tg_;                // Base's data member:
}

template <Tag__ tg_>
Semantic<tg_>::Semantic(Semantic<tg_> const &other)
:
    d_data(other.d_data)
{
    d_baseTag = other.d_baseTag;
}

template <Tag__ tg_>
template <typename ...Params>
Semantic<tg_>::Semantic(Params &&...params)
:
    d_data(std::forward<Params>(params) ...)
{
    d_baseTag = tg_;
}


template <Tag__ tg_>
Base *Semantic<tg_>::vClone() const
{
    return new Semantic<tg_>{*this};
}

template <Tag__ tg_>
void *Semantic<tg_>::vData() const 
{
    return const_cast<typename TypeOf<tg_>::type *>(&d_data);
}


    // The class SType wraps a pointer to Base.  It becomes the polymorphic
    // STYPE__ type. It also defines get members, allowing constructions like
    // $$.get<INT> to be used.  
class SType: private std::unique_ptr<Base>
{
    typedef std::unique_ptr<Base> BasePtr;

    public:
        SType() = default;
        SType(SType const &other);
        SType(SType &&tmp);

        ~SType() = default;

            // Specific overloads are needed for SType = SType assignments
        SType &operator=(SType const &rhs);
        SType &operator=(SType &rhs);           // required so it is used
                                                // instead of the template op=
        SType &operator=(SType &&tmp);

            // A template member operator= can be used when the compiler is
            // able to deduce the appropriate typename. Otherwise use assign.
        template <typename Type>
        SType &operator=(Type const &value);

        template <typename Type>                // same, now moving
        SType &operator=(Type &&tmp);

        template <Tag__ tag, typename ...Args>
        void assign(Args &&...args);
    
            // By default the get()-members check whether the specified <tag>
            // matches the tag returned by SType::tag (d_data's tag). If they
            // don't match a run-time fatal error results.
        template <Tag__ tag>
        typename TypeOf<tag>::type &get();

        template <Tag__ tag>
        typename TypeOf<tag>::type const &get() const;

        Tag__ tag() const;
        bool valid() const;
};

inline SType::SType(SType const &other)
:
    BasePtr{other ? other->clone() : 0}
{}

inline SType::SType(SType &&tmp)
:
    BasePtr{std::move(tmp)}
{}

inline SType &SType::operator=(SType const &rhs)
{
    reset(rhs->clone());
    return *this;
}

inline SType &SType::operator=(SType &rhs)
{
    reset(rhs->clone());
    return *this;
}

inline SType &SType::operator=(SType &&tmp)
{
    BasePtr::operator=(std::move(tmp));
    return *this;
}

    // A template assignment function can be used when the compiler is 
    // able to deduce the appropriate typename
template <typename Type>
inline SType &SType::operator=(Type const &value)
{
    assign< TagOf<Type>::tag >(value);
    return *this;
}

template <typename Type>
inline SType &SType::operator=(Type &&tmp)
{
    assign< 
        TagOf<
            typename std::remove_reference<Type>::type
        >::tag 
    >(std::move(tmp));

    return *this;
}

template <Tag__ tag, typename ...Args>
void SType::assign(Args &&...args)
{
    reset(new Semantic<tag>(std::forward<Args>(args) ...));
}

template <Tag__ tg>
typename TypeOf<tg>::type &SType::get()
{
// $insert warnTagMismatches

    if (tag() != tg)
    {
        std::cerr << "[Fatal] calling `.get<Tag__::" << 
            idOfTag__[static_cast<int>(tg)] << 
            ">()', but Tag " <<
            idOfTag__[static_cast<int>(tag())] << " is encountered. "
            " Try option --debug and call setDebug(Parser::ACTIONCASES)\n";
        throw 1;        // ABORTs
    }
    
    return *static_cast<typename TypeOf<tg>::type *>( (*this)->data() );
}

template <Tag__ tg>
typename TypeOf<tg>::type const &SType::get() const
{
// $insert warnTagMismatches

    if (tag() != tg)
    {
        std::cerr << "[Fatal] calling `.get<Tag__::" << 
            idOfTag__[static_cast<int>(tg)] << 
            ">()', but Tag " <<
            idOfTag__[static_cast<int>(tag())] << " is encountered. "
            " Try option --debug and call setDebug(Parser::ACTIONCASES)\n";
        throw 1;        // ABORTs
    }
    
    return *static_cast<typename TypeOf<tg>::type *>( (*this)->data() );
}

inline Tag__ SType::tag() const
{
    return valid() ? (*this)->tag() : static_cast<Tag__>(sizeofTag__);
}

inline bool SType::valid() const
{
    return BasePtr::get() != 0;
}

}  // namespace Meta__

class ParserBase
{
    public:
        enum DebugMode__
        {
            OFF           = 0,
            ON            = 1 << 0,
            ACTIONCASES   = 1 << 1
        };

// $insert tokens

    // Symbolic tokens:
    enum Tokens__
    {
        BASECLASS_HEADER = 257,
        BASECLASS_PREINCLUDE,
        BLOCK,
        CLASS_HEADER,
        CLASS_NAME,
        CONSTRUCTOR_CHECKS,
        DEBUGFLAG,
        DEFAULT_ACTIONS,
        ERROR_VERBOSE,
        EXPECT,
        FILENAMES,
        FLEX,
        IDENTIFIER,
        IMPLEMENTATION_HEADER,
        LEFT,
        LOCATIONSTRUCT,
        LSP_NEEDED,
        LTYPE,
        NAMESPACE,
        NEG_DOLLAR,
        NOLINES,
        NONASSOC,
        NUMBER,
        PARSEFUN_SOURCE,
        POLYMORPHIC,
        PREC,
        PRINT_TOKENS,
        QUOTE,
        REQUIRED,
        RIGHT,
        SCANNER,
        SCANNER_CLASS_NAME,
        SCANNER_MATCHED_TEXT_FUNCTION,
        SCANNER_TOKEN_FUNCTION,
        STACK_EXPANSION,
        START,
        STRING,
        STYPE,
        TARGET_DIRECTORY,
        TOKEN,
        TWO_PERCENTS,
        TYPE,
        UNION,
        WARN_TAGS,
        WEAK_TAGS,
    };

// $insert STYPE
    typedef Meta__::SType STYPE__;


    private:
        int d_stackIdx__ = -1;
        std::vector<size_t>   d_stateStack__;
        std::vector<STYPE__>  d_valueStack__;

    protected:
        enum Return__
        {
            PARSE_ACCEPT__ = 0,   // values used as parse()'s return values
            PARSE_ABORT__  = 1
        };
        enum ErrorRecovery__
        {
            DEFAULT_RECOVERY_MODE__,
            UNEXPECTED_TOKEN__,
        };
        bool        d_actionCases__ = false;
        bool        d_debug__ = true;
        size_t      d_nErrors__ = 0;
        size_t      d_requiredTokens__;
        size_t      d_acceptedTokens__;
        int         d_token__;
        int         d_nextToken__;
        size_t      d_state__;
        STYPE__    *d_vsp__;
        STYPE__     d_val__;
        STYPE__     d_nextVal__;

        ParserBase();

// $insert debugdecl
        static std::ostringstream s_out__;
        
        std::string symbol__(int value) const;
        std::string stype__(char const *pre, STYPE__ const &semVal,
                            char const *post = "") const;
        static std::ostream &dflush__(std::ostream &out);
        void ABORT() const;
        void ACCEPT() const;
        void ERROR() const;
        void clearin();
        bool actionCases() const;
        bool debug() const;
        void pop__(size_t count = 1);
        void push__(size_t nextState);
        void popToken__();
        void pushToken__(int token);
        void reduce__(PI__ const &productionInfo);
        void errorVerbose__();
        size_t top__() const;

    public:
        void setDebug(bool mode);
        void setDebug(DebugMode__ mode);
}; 

inline ParserBase::DebugMode__ operator|(ParserBase::DebugMode__ lhs, 
                                     ParserBase::DebugMode__ rhs)
{
    return static_cast<ParserBase::DebugMode__>(static_cast<int>(lhs) | rhs);
};

inline bool ParserBase::debug() const
{
    return d_debug__;
}

inline bool ParserBase::actionCases() const
{
    return d_actionCases__;
}

inline void ParserBase::ABORT() const
{
    // $insert debug
    if (d_debug__)
        s_out__ <<  "ABORT(): Parsing unsuccessful" << "\n" << dflush__;
    throw PARSE_ABORT__;
}

inline void ParserBase::ACCEPT() const
{
    // $insert debug
    if (d_debug__)
        s_out__ <<  "ACCEPT(): Parsing successful" << "\n" << dflush__;
    throw PARSE_ACCEPT__;
}

inline void ParserBase::ERROR() const
{
    // $insert debug
    if (d_debug__)
        s_out__ <<  "ERROR(): Forced error condition" << "\n" << dflush__;
    throw UNEXPECTED_TOKEN__;
}

inline ParserBase::DebugMode__ operator&(ParserBase::DebugMode__ lhs,
                                     ParserBase::DebugMode__ rhs)
{
    return static_cast<ParserBase::DebugMode__>(
            static_cast<int>(lhs) & rhs);
}

// For convenience, when including ParserBase.h its symbols are available as
// symbols in the class Parser, too.
#define Parser ParserBase


#endif


