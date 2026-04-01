########
# Add framework directory including bsps and support files
add_subdirectory(framework framework)
########

## TODO: Add selection to enable/disable different benchmark suites.  Only one allowed at a time

# Add MLPerfTiny benchmark directory
add_subdirectory(MLPerfTiny MLPerfTiny)

########
# To add new benchmark suites, add the new suite folder along with a flag to select it from the command line
########

