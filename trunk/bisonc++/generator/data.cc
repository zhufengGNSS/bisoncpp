#include "generator.ih"

Generator::Map Generator::s_insert =
{
    {"actioncases",         &Generator::actionCases},
    {"lex",                 &Generator::lex},
    {"scanner.h",           &Generator::scannerH},
    {"scannerobject",       &Generator::scannerObject},
    {"baseclass",           &Generator::baseClass},
    {"class.h",             &Generator::classH},
    {"class.ih",            &Generator::classIH},
    {"debug",               &Generator::debug},
    {"debugdecl",           &Generator::debugDecl},
    {"debuginit",           &Generator::debugInit},
    {"debugfunctions",      &Generator::debugFunctions},
    {"debugincludes",       &Generator::debugIncludes},
    {"debuglookup",         &Generator::debugLookup},
    {"errorverbose",        &Generator::errorVerbose},
    {"namespace-open",      &Generator::namespaceOpen},
    {"namespace-close",     &Generator::namespaceClose},
    {"namespace-use",       &Generator::namespaceUse},
    {"preincludes",         &Generator::preIncludes},
    {"print",               &Generator::print},
    {"requiredtokens",      &Generator::requiredTokens},
    {"staticdata",          &Generator::staticData},
    {"threading",           &Generator::threading},
    {"tokens",              &Generator::tokens},
    {"LTYPE",               &Generator::ltype},
    {"LTYPEdata",           &Generator::ltypeData},
    {"LTYPEpop",            &Generator::ltypePop},
    {"LTYPEpush",           &Generator::ltypePush},
    {"LTYPEresize",         &Generator::ltypeResize},
    {"LTYPEstack",          &Generator::ltypeStack},
    {"STYPE",               &Generator::stype},
};

char const *Generator::s_baseFlag = "\\@";
size_t const Generator::s_baseFlagSize = 2; // backslash and @

string Generator::s_matchedTextFunction = "d_scanner.YYText()";
string Generator::s_scannerTokenFunction = "d_scanner.yylex()";

