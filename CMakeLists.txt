cmake_minimum_required(VERSION 3.15)

project(REDSP_TESTS VERSION 0.0.1)

add_subdirectory(./ext/JUCE)                    # If you've put JUCE in a subdirectory called JUCE

juce_add_console_app(redsp_test
        PRODUCT_NAME "redsp_test")     # The name of the final executable, which can differ from the target name

target_sources(redsp_test
        PRIVATE
        ./test/main.cpp)

target_compile_definitions(redsp_test
        PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_console_app` call
        JUCE_USE_CURL=0)    # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_console_app` call

target_link_libraries(redsp_test
        PRIVATE
        # ConsoleAppData            # If you'd created a binary data target, you'd link to it here
        juce::juce_core
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)
