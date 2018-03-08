#ifndef CPPSERIALPORT_CPPSERIALPORTCHAISCRIPTMODULEFACTORY_H
#define CPPSERIALPORT_CPPSERIALPORTCHAISCRIPTMODULEFACTORY_H

#include "ChaiScriptModuleFactory.h"

namespace CppSerialPort {

    class CppSerialPortChaiScriptModuleFactory : public ChaiScriptModuleFactory {
    public:
        chaiscript::ModulePtr createModule() override;

    };

} //namespace CppSerialPort


#endif //CPPSERIALPORT_CPPSERIALPORTCHAISCRIPTMODULEFACTORY_H
