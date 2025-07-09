project('pong', 'c', meson_version : '>=1.8.0')

python = find_program('python')
glslc = find_program('glslc')

shader_sources = [
  'shaders/base.vert',
  'shaders/base.frag',
]

shaders = custom_target(
  'shaders',
  depend_files : shader_sources,
  input : shader_sources,
  output : [ 'embed.h', 'embed.c' ],
  command : [ python, '@SOURCE_ROOT@/embed.py', glslc, '@PRIVATE_DIR@', '@OUTPUT@', '@INPUT@' ],
  build_by_default : true
)

wayland = import('wayland')

xdg_shell = wayland.scan_xml( wayland.find_protocol('xdg-shell') )
xdg_decoration = wayland.scan_xml( wayland.find_protocol('xdg-decoration', state : 'unstable', version : 1) )

sources = [
  'src/entry.c',
  'src/memory.c',
  'src/pong.c',

  'src/window/window.c',
  'src/window/display.c',
  'src/window/xdg_surface.c',
  'src/window/xdg_toplevel.c',
  'src/window/xdg_decoration.c',
  'src/window/vulkan.c',
  'src/window/keyboard.c',

  'src/vulkan/vulkan.c',
  'src/vulkan/instance.c',
  'src/vulkan/physical.c',
  'src/vulkan/device.c',
  'src/vulkan/surface.c',
  'src/vulkan/swapchain.c',
  'src/vulkan/shaders.c',
  'src/vulkan/descriptor.c',
  'src/vulkan/pipeline_layout.c',
  'src/vulkan/render_pass.c',
  'src/vulkan/pipeline.c',
  'src/vulkan/images.c',
  'src/vulkan/views.c',
  'src/vulkan/framebuffers.c',
  'src/vulkan/command_pool.c',
  'src/vulkan/command_buffer.c',
  'src/vulkan/sync.c',
  'src/vulkan/buffer.c',

  xdg_shell,
  xdg_decoration,
  shaders
]

includes = include_directories('include')

dependencies = [
  dependency('wayland-client'),
  dependency('vulkan'),
  dependency('xkbcommon')
]

executable('pong', sources : sources, include_directories : includes, dependencies: dependencies)
