# Format files macro

macro(formatter src ext)
    file(GLOB files RELATIVE ${src} ${src}/*.${ext})
    message(STATUS "Files to modify: ${files}")
    foreach(file ${files})
        set(fullpathsrc "${src}/${file}")
        if(IS_DIRECTORY ${fullpathsrc})
            formatter(${fullpathsrc} ext)
        else(IS_DIRECTORY ${fullpathsrc})
            message("    [INFO] Formatting ${fullpathsrc} ")
            execute_process(COMMAND "clang-format" "-i" "-style=file" "${fullpathsrc}")
        endif(IS_DIRECTORY ${fullpathsrc})
    endforeach(file)
endmacro(formatter)

set(SRC "${CMAKE_ARGV3}")
set(EXT "${CMAKE_ARGV4}")

formatter(${SRC} ${EXT})
  
