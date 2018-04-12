#ifndef CPPSERIALPORT_CPPSERIALPORTCHAISCRIPTMODULEFACTORY_HPP
#define CPPSERIALPORT_CPPSERIALPORTCHAISCRIPTMODULEFACTORY_HPP

#include "ChaiScriptModuleFactory.hpp"

namespace CppSerialPort {

    class CppSerialPortChaiScriptModuleFactory : public ChaiScriptModuleFactory {
    public:
        chaiscript::ModulePtr createModule() override;

    };

} //namespace CppSerialPort


#endif //CPPSERIALPORT_CPPSERIALPORTCHAISCRIPTMODULEFACTORY_HPP
