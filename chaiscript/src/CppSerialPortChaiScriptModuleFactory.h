#ifndef TRMIDOUBLECHECKBOX_CPPSERIALPORTChaiScriptModuleFactory_H
#define TRMIDOUBLECHECKBOX_CPPSERIALPORTChaiScriptModuleFactory_H

#include "ChaiScriptModuleFactory.h"

class CppSerialPortChaiScriptModuleFactory : public CSP_ChaiScriptModuleFactory
{
public:
    chaiscript::ModulePtr createModule() override;

};


#endif //TRMIDOUBLECHECKBOX_CPPSERIALPORTChaiScriptModuleFactory_H
