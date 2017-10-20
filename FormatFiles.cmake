# Format files macro

macro(formatter src)
    file(GLOB files RELATIVE ${src} ${src}/*)
    message(STATUS "Source folder: [${src}]")
    foreach(file ${files})
        set(fullpathsrc "${src}/${file}")
        if(IS_DIRECTORY ${fullpathsrc})
            formatter(${fullpathsrc} ext)
        else(IS_DIRECTORY ${fullpathsrc})
            get_filename_component(extension ${fullpathsrc} EXT)
            if(${extension} STREQUAL ".h")
                message("    [INFO] Modifying header file: ${fullpathsrc}")
                execute_process(COMMAND "clang-format" "-i" "-style=file" "${fullpathsrc}")
            endif(${extension} STREQUAL ".h")
            if(${extension} STREQUAL ".cc")
                message("    [INFO] Modifying source file: ${fullpathsrc}")
                execute_process(COMMAND "clang-format" "-i" "-style=file" "${fullpathsrc}")
            endif(${extension} STREQUAL ".cc")
        endif(IS_DIRECTORY ${fullpathsrc})
    endforeach(file)
endmacro(formatter)

set(SRC "${CMAKE_ARGV3}")
set(EXT "${CMAKE_ARGV4}")

formatter(${SRC} ${EXT})
  
