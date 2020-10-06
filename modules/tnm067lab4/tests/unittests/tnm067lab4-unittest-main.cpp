#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif

#include <inviwo/core/common/coremodulesharedlibrary.h>
#include <inviwo/core/common/inviwoapplication.h>

#include <warn/push>
#include <warn/ignore/all>
#include <gtest/gtest.h>
#include <warn/pop>

#include <iostream>

using namespace inviwo;

int main(int argc, char **argv)
{
    inviwo::LogCentral::init();
    InviwoApplication app(argc, argv, "Inviwo-Unittests-TNM067-Lab4");
    {
        std::vector<std::unique_ptr<InviwoModuleFactoryObject>> modules;
        modules.emplace_back(createInviwoCore());
        app.registerModules(std::move(modules));
    }

    int ret = -1;
    {
        ::testing::InitGoogleTest(&argc, argv);
        ret = RUN_ALL_TESTS();
    }

    std::cout << "Press any key to exit ..." << std::endl;
    std::cin.get();

    return ret;
}