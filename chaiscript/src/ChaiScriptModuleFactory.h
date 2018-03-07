#ifndef CPPSERIALPORT_CHAISCRIPTMMODULEFACTORY_H
#define CPPSERIALPORT_CHAISCRIPTMMODULEFACTORY_H

#include <chaiscript/chaiscript.hpp>


class CSP_ChaiScriptModuleFactory
{
public:
    virtual chaiscript::ModulePtr createModule() = 0;
    virtual void exportGlobalInstanceVariables(chaiscript::ChaiScript *chaiEngine) {
        (void)chaiEngine;
        //No global variables to inject
    }

};


#endif //CPPSERIALPORT_CHAISCRIPTMMODULEFACTORY_H
