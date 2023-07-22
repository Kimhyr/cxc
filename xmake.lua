add_rules('mode.debug', 'mode.release')

set_warnings('everything')
set_toolchains('clang')
set_languages('c++20')

add_cxxflags(
	'-Wno-switch-enum',
	'-Wno-c++98-compat',
	'-Wno-c++98-compat-pedantic',
	'-Wno-c++20-compat',
	'-Wno-pre-c++20-compat',
	'-Wno-pre-c++20-compat-pedantic',
	'-Wno-c++23-extensions',
	'-Wno-c99-designator',
	'-Wno-unsafe-buffer-usage',
	'-stdlib=libstdc++'
)

add_includedirs('.')

if is_mode('debug') then
	set_symbols('debug')
	set_optimize('none')
else
	set_symbols('none')
end

target('cxc')
	set_kind('binary')
	add_files('cxc/**.cpp')
