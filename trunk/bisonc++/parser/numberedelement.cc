#include "parser.ih"

// pos points to $ in what must be $i

bool Parser::numberedElement(size_t pos, int nElements, Block &block) 
{
    int idx;                // extract the index, determine its length
    size_t length  = 1 + extractIndex(&idx, pos + 1);

    string const &idxType = d_rules.sType(idx);

    size_t ruleElements = nComponents(nElements);

    if (idx > static_cast<int>(ruleElements)) // $i refers beyond this rule
        lineMsg(emsg) << "In production rule \n"<< 
                    "\t`" << &d_rules.lastProduction() << " '\n"
                    "\t$" << idx << " used, but there are only " << 
                    ruleElements << " elements" << endl;
    else if (d_unionDeclared)
    {
        if (idx <= 0)                   // type of the $i can't be determined
        {                               // and indices <= 0 are not accepted
            if (!d_negativeDollarIndices)
                lineMsg(wmsg) << "In production rule \n"<< 
                            "\t`" << &d_rules.lastProduction() << " '\n"
                            "\tcannot determine default type of $" << 
                            idx << endl;
        }
        else if (!idxType.length())     // or $i without type association
                lineMsg(wmsg) << "In production rule \n"<< 
                            "\t`" << &d_rules.lastProduction() << " '\n"
                            "\t$" << idx << " (" << d_rules.symbol(idx) << 
                            ") has no default type-association" << endl;
    }
    else if (!d_negativeDollarIndices && idx <= 0)
        lineMsg(wmsg) << "In production rule \n"<< 
                    "\t`" << &d_rules.lastProduction() << " '\n"
                    "\tnon-positive $" << idx << " used" << endl;

    ostringstream os;
    os << s_semanticValueStack << "[" << indexToOffset(idx, nElements) << "]";
    string replacement = os.str();

    if (idxType.length())
        replacement += "." + idxType;

    block.replace(pos, length, replacement);

    return false;           // this is not a $$ variant.
}