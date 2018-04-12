#ifndef CPPSERIALPORT_CHAISCRIPTMMODULEFACTORY_HPP
#define CPPSERIALPORT_CHAISCRIPTMMODULEFACTORY_HPP

#include <chaiscript/chaiscript.hpp>


namespace CppSerialPort {

    class ChaiScriptModuleFactory {
    public:
        virtual chaiscript::ModulePtr createModule() = 0;

        virtual void exportGlobalInstanceVariables(chaiscript::ChaiScript *chaiEngine) {
            (void) chaiEngine;
            //No global variables to inject
        }

    };

} //namespace CppSerialPort


#endif //CPPSERIALPORT_CHAISCRIPTMMODULEFACTORY_HPP
