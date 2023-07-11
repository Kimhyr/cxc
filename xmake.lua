add_rules('mode.debug', 'mode.release')

set_warnings('everything')
set_toolchains('clang')

add_cxxflags(
	'-Wno-c++98-compat',
	'-Wno-pre-c++20-compat',
	'-Wno-pre-c++20-compat-pedantic',
	'-fmodules',
	'-stdlib=libstdc++',
	-- '-fmodule-map-file=source/module.modulemap'
	'-fbuiltin-module-map'
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
	add_files('source/**.c')
