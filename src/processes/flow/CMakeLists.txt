project(sprokit_processes_flow)

set(flow_srcs
  collate_process.cxx
  distribute_process.cxx
  pass_process.cxx
  registration.cxx
  sink_process.cxx)

set(flow_private_headers
  collate_process.h
  distribute_process.h
  flow-config.h
  pass_process.h
  registration.h
  sink_process.h)

sprokit_private_header_group(${flow_private_headers})
sprokit_add_plugin(processes_flow
  MAKE_SPROKIT_PROCESSES_FLOW_LIB
  ${flow_srcs}
  ${flow_private_headers})
target_link_libraries(processes_flow
  LINK_PRIVATE
    sprokit_pipeline)
