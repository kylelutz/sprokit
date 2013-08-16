/*ckwg +5
 * Copyright 2011-2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <test_common.h>

#include <sprokit/pipeline/config.h>
#include <sprokit/pipeline/modules.h>
#include <sprokit/pipeline/pipeline.h>
#include <sprokit/pipeline/pipeline_exception.h>
#include <sprokit/pipeline/process.h>
#include <sprokit/pipeline/process_cluster.h>
#include <sprokit/pipeline/process_exception.h>
#include <sprokit/pipeline/process_registry.h>
#include <sprokit/pipeline/scheduler.h>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/make_shared.hpp>

#define TEST_ARGS ()

DECLARE_TEST(null_config);
DECLARE_TEST(null_process);
DECLARE_TEST(add_process);
DECLARE_TEST(add_cluster);
DECLARE_TEST(duplicate_process_process);
DECLARE_TEST(connect_no_upstream);
DECLARE_TEST(connect_no_downstream);
DECLARE_TEST(connect_untyped_data_connection);
DECLARE_TEST(connect_untyped_flow_connection);
DECLARE_TEST(connect_type_mismatch);
DECLARE_TEST(connect_flag_shared_no_mutate);
DECLARE_TEST(connect_flag_mismatch_const_mutate);
DECLARE_TEST(connect_flag_mismatch_shared_mutate_first);
DECLARE_TEST(connect_flag_mismatch_shared_mutate_second);
DECLARE_TEST(connect);
DECLARE_TEST(setup_pipeline_no_processes);
DECLARE_TEST(setup_pipeline_orphaned_process);
DECLARE_TEST(setup_pipeline_type_force_flow_upstream);
DECLARE_TEST(setup_pipeline_type_force_flow_downstream);
DECLARE_TEST(setup_pipeline_type_force_cascade_up);
DECLARE_TEST(setup_pipeline_type_force_cascade_down);
DECLARE_TEST(setup_pipeline_type_force_cascade_both);
DECLARE_TEST(setup_pipeline_backwards_edge);
DECLARE_TEST(setup_pipeline_not_a_dag);
DECLARE_TEST(setup_pipeline_data_dependent_set);
DECLARE_TEST(setup_pipeline_data_dependent_set_reject);
DECLARE_TEST(setup_pipeline_data_dependent_set_cascade);
DECLARE_TEST(setup_pipeline_data_dependent_set_cascade_reject);
DECLARE_TEST(setup_pipeline_type_force_flow_upstream_reject);
DECLARE_TEST(setup_pipeline_type_force_flow_downstream_reject);
DECLARE_TEST(setup_pipeline_type_force_cascade_reject);
DECLARE_TEST(setup_pipeline_untyped_data_dependent);
DECLARE_TEST(setup_pipeline_untyped_connection);
DECLARE_TEST(setup_pipeline_missing_required_input_connection);
DECLARE_TEST(setup_pipeline_missing_required_output_connection);
DECLARE_TEST(setup_pipeline_duplicate);
DECLARE_TEST(setup_pipeline_add_process);
DECLARE_TEST(setup_pipeline_connect);
DECLARE_TEST(setup_pipeline);
DECLARE_TEST(start_before_setup);
DECLARE_TEST(start_unsuccessful_setup);
DECLARE_TEST(start_and_stop);
DECLARE_TEST(reset_while_running);
DECLARE_TEST(reset);
DECLARE_TEST(remove_process);
DECLARE_TEST(remove_process_after_setup);
DECLARE_TEST(disconnect);
DECLARE_TEST(disconnect_after_setup);
DECLARE_TEST(reconfigure_before_setup);
DECLARE_TEST(reconfigure);
DECLARE_TEST(reconfigure_only_top_level);

int
main(int argc, char* argv[])
{
  CHECK_ARGS(1);

  testname_t const testname = argv[1];

  DECLARE_TEST_MAP(tests);

  ADD_TEST(tests, null_config);
  ADD_TEST(tests, null_process);
  ADD_TEST(tests, add_process);
  ADD_TEST(tests, add_cluster);
  ADD_TEST(tests, duplicate_process_process);
  ADD_TEST(tests, connect_no_upstream);
  ADD_TEST(tests, connect_no_downstream);
  ADD_TEST(tests, connect_untyped_data_connection);
  ADD_TEST(tests, connect_untyped_flow_connection);
  ADD_TEST(tests, connect_type_mismatch);
  ADD_TEST(tests, connect_flag_shared_no_mutate);
  ADD_TEST(tests, connect_flag_mismatch_const_mutate);
  ADD_TEST(tests, connect_flag_mismatch_shared_mutate_first);
  ADD_TEST(tests, connect_flag_mismatch_shared_mutate_second);
  ADD_TEST(tests, connect);
  ADD_TEST(tests, setup_pipeline_no_processes);
  ADD_TEST(tests, setup_pipeline_orphaned_process);
  ADD_TEST(tests, setup_pipeline_type_force_flow_upstream);
  ADD_TEST(tests, setup_pipeline_type_force_flow_downstream);
  ADD_TEST(tests, setup_pipeline_type_force_cascade_up);
  ADD_TEST(tests, setup_pipeline_type_force_cascade_down);
  ADD_TEST(tests, setup_pipeline_type_force_cascade_both);
  ADD_TEST(tests, setup_pipeline_backwards_edge);
  ADD_TEST(tests, setup_pipeline_not_a_dag);
  ADD_TEST(tests, setup_pipeline_data_dependent_set);
  ADD_TEST(tests, setup_pipeline_data_dependent_set_reject);
  ADD_TEST(tests, setup_pipeline_data_dependent_set_cascade);
  ADD_TEST(tests, setup_pipeline_data_dependent_set_cascade_reject);
  ADD_TEST(tests, setup_pipeline_type_force_flow_upstream_reject);
  ADD_TEST(tests, setup_pipeline_type_force_flow_downstream_reject);
  ADD_TEST(tests, setup_pipeline_type_force_cascade_reject);
  ADD_TEST(tests, setup_pipeline_untyped_data_dependent);
  ADD_TEST(tests, setup_pipeline_untyped_connection);
  ADD_TEST(tests, setup_pipeline_missing_required_input_connection);
  ADD_TEST(tests, setup_pipeline_missing_required_output_connection);
  ADD_TEST(tests, setup_pipeline_duplicate);
  ADD_TEST(tests, setup_pipeline_add_process);
  ADD_TEST(tests, setup_pipeline_connect);
  ADD_TEST(tests, setup_pipeline);
  ADD_TEST(tests, start_before_setup);
  ADD_TEST(tests, start_unsuccessful_setup);
  ADD_TEST(tests, start_and_stop);
  ADD_TEST(tests, reset_while_running);
  ADD_TEST(tests, reset);
  ADD_TEST(tests, remove_process);
  ADD_TEST(tests, remove_process_after_setup);
  ADD_TEST(tests, disconnect);
  ADD_TEST(tests, disconnect_after_setup);
  ADD_TEST(tests, reconfigure_before_setup);
  ADD_TEST(tests, reconfigure);
  ADD_TEST(tests, reconfigure_only_top_level);

  RUN_TEST(tests, testname);
}

static sprokit::process_t create_process(sprokit::process::type_t const& type, sprokit::process::name_t const& name, sprokit::config_t config = sprokit::config::empty_config());
static sprokit::pipeline_t create_pipeline();

IMPLEMENT_TEST(null_config)
{
  sprokit::config_t const config;

  EXPECT_EXCEPTION(sprokit::null_pipeline_config_exception,
                   boost::make_shared<sprokit::pipeline>(config),
                   "passing a NULL config to the pipeline");
}

IMPLEMENT_TEST(null_process)
{
  sprokit::process_t const process;

  sprokit::pipeline_t const pipeline = create_pipeline();

  EXPECT_EXCEPTION(sprokit::null_process_addition_exception,
                   pipeline->add_process(process),
                   "adding a NULL process to the pipeline");
}

IMPLEMENT_TEST(add_process)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("numbers");

  sprokit::process_t const process = create_process(proc_type, sprokit::process::name_t());

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
}

IMPLEMENT_TEST(add_cluster)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("multiplier_cluster");

  sprokit::process_t const process = create_process(proc_type, sprokit::process::name_t());

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);

  sprokit::process::names_t const names = pipeline->process_names();

  if (names.size() != 2)
  {
    TEST_ERROR("Improperly adding clusters to the pipeline");
  }
}

IMPLEMENT_TEST(duplicate_process_process)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("numbers");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("name");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const dup_process = create_process(proc_type, proc_name);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);

  EXPECT_EXCEPTION(sprokit::duplicate_process_name_exception,
                   pipeline->add_process(dup_process),
                   "adding a duplicate process to the pipeline");
}

IMPLEMENT_TEST(connect_no_upstream)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("numbers");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("name");

  sprokit::process_t const process = create_process(proc_type, proc_name);

  sprokit::pipeline_t const pipeline = create_pipeline();

  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("othername");

  pipeline->add_process(process);

  EXPECT_EXCEPTION(sprokit::no_such_process_exception,
                   pipeline->connect(proc_name2, sprokit::process::port_t(),
                                     proc_name, sprokit::process::port_t()),
                   "connecting with a non-existent upstream");
}

IMPLEMENT_TEST(connect_no_downstream)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("numbers");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("name");

  sprokit::process_t const process = create_process(proc_type, proc_name);

  sprokit::pipeline_t const pipeline = create_pipeline();

  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("othername");

  pipeline->add_process(process);

  EXPECT_EXCEPTION(sprokit::no_such_process_exception,
                   pipeline->connect(proc_name, sprokit::process::port_t(),
                                     proc_name2, sprokit::process::port_t()),
                   "connecting with a non-existent downstream");
}

IMPLEMENT_TEST(connect_untyped_data_connection)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("data_dependent");
  sprokit::process::type_t const proc_type2 = sprokit::process::type_t("flow_dependent");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("data");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("sink");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const process2 = create_process(proc_type2, proc_name2);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);

  sprokit::process::port_t const port_name = sprokit::process::port_t("output");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("input");

  pipeline->connect(proc_name, port_name,
                    proc_name2, port_name2);
}

IMPLEMENT_TEST(connect_untyped_flow_connection)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("flow_dependent");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("up");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("down");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const process2 = create_process(proc_type, proc_name2);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);

  sprokit::process::port_t const port_name = sprokit::process::port_t("output");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("input");

  pipeline->connect(proc_name, port_name,
                    proc_name2, port_name2);
}

IMPLEMENT_TEST(connect_type_mismatch)
{
  sprokit::process::type_t const proc_typeu = sprokit::process::type_t("numbers");
  sprokit::process::type_t const proc_typed = sprokit::process::type_t("take_string");

  sprokit::process::name_t const proc_nameu = sprokit::process::name_t("upstream");
  sprokit::process::name_t const proc_named = sprokit::process::name_t("downstream");

  sprokit::process_t const processu = create_process(proc_typeu, proc_nameu);
  sprokit::process_t const processd = create_process(proc_typed, proc_named);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(processu);
  pipeline->add_process(processd);

  sprokit::process::port_t const port_nameu = sprokit::process::port_t("number");
  sprokit::process::port_t const port_named = sprokit::process::port_t("string");

  EXPECT_EXCEPTION(sprokit::connection_type_mismatch_exception,
                   pipeline->connect(proc_nameu, port_nameu,
                                     proc_named, port_named),
                   "connecting type-mismatched ports");
}

IMPLEMENT_TEST(connect_flag_shared_no_mutate)
{
  sprokit::process::type_t const proc_typeu = sprokit::process::type_t("shared");
  sprokit::process::type_t const proc_typed = sprokit::process::type_t("sink");

  sprokit::process::name_t const proc_nameu = sprokit::process::name_t("upstream");
  sprokit::process::name_t const proc_named1 = sprokit::process::name_t("downstream1");
  sprokit::process::name_t const proc_named2 = sprokit::process::name_t("downstream2");

  sprokit::process_t const processu = create_process(proc_typeu, proc_nameu);
  sprokit::process_t const processd1 = create_process(proc_typed, proc_named1);
  sprokit::process_t const processd2 = create_process(proc_typed, proc_named2);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(processu);
  pipeline->add_process(processd1);
  pipeline->add_process(processd2);

  sprokit::process::port_t const port_nameu = sprokit::process::port_t("shared");
  sprokit::process::port_t const port_named = sprokit::process::port_t("sink");

  pipeline->connect(proc_nameu, port_nameu,
                    proc_named1, port_named);
  pipeline->connect(proc_nameu, port_nameu,
                    proc_named2, port_named);
}

IMPLEMENT_TEST(connect_flag_mismatch_const_mutate)
{
  sprokit::process::type_t const proc_typeu = sprokit::process::type_t("const");
  sprokit::process::type_t const proc_typed = sprokit::process::type_t("mutate");

  sprokit::process::name_t const proc_nameu = sprokit::process::name_t("upstream");
  sprokit::process::name_t const proc_named = sprokit::process::name_t("downstream");

  sprokit::process_t const processu = create_process(proc_typeu, proc_nameu);
  sprokit::process_t const processd = create_process(proc_typed, proc_named);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(processu);
  pipeline->add_process(processd);

  sprokit::process::port_t const port_nameu = sprokit::process::port_t("const");
  sprokit::process::port_t const port_named = sprokit::process::port_t("mutate");

  EXPECT_EXCEPTION(sprokit::connection_flag_mismatch_exception,
                   pipeline->connect(proc_nameu, port_nameu,
                                     proc_named, port_named),
                   "connecting a const to a mutate port");
}

IMPLEMENT_TEST(connect_flag_mismatch_shared_mutate_first)
{
  sprokit::process::type_t const proc_typeu = sprokit::process::type_t("shared");
  sprokit::process::type_t const proc_typed = sprokit::process::type_t("sink");
  sprokit::process::type_t const proc_typem = sprokit::process::type_t("mutate");

  sprokit::process::name_t const proc_nameu = sprokit::process::name_t("upstream");
  sprokit::process::name_t const proc_named = sprokit::process::name_t("downstream");
  sprokit::process::name_t const proc_namem = sprokit::process::name_t("downstream_mutate");

  sprokit::process_t const processu = create_process(proc_typeu, proc_nameu);
  sprokit::process_t const processd = create_process(proc_typed, proc_named);
  sprokit::process_t const processm = create_process(proc_typem, proc_namem);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(processu);
  pipeline->add_process(processd);
  pipeline->add_process(processm);

  sprokit::process::port_t const port_nameu = sprokit::process::port_t("shared");
  sprokit::process::port_t const port_named = sprokit::process::port_t("sink");
  sprokit::process::port_t const port_namem = sprokit::process::port_t("mutate");

  pipeline->connect(proc_nameu, port_nameu,
                    proc_namem, port_namem);

  EXPECT_EXCEPTION(sprokit::connection_flag_mismatch_exception,
                   pipeline->connect(proc_nameu, port_nameu,
                                     proc_named, port_named),
                   "connecting to a shared port already connected to a mutate port");
}

IMPLEMENT_TEST(connect_flag_mismatch_shared_mutate_second)
{
  sprokit::process::type_t const proc_typeu = sprokit::process::type_t("shared");
  sprokit::process::type_t const proc_typed = sprokit::process::type_t("sink");
  sprokit::process::type_t const proc_typem = sprokit::process::type_t("mutate");

  sprokit::process::name_t const proc_nameu = sprokit::process::name_t("upstream");
  sprokit::process::name_t const proc_named = sprokit::process::name_t("downstream");
  sprokit::process::name_t const proc_namem = sprokit::process::name_t("downstream_mutate");

  sprokit::process_t const processu = create_process(proc_typeu, proc_nameu);
  sprokit::process_t const processd = create_process(proc_typed, proc_named);
  sprokit::process_t const processm = create_process(proc_typem, proc_namem);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(processu);
  pipeline->add_process(processd);
  pipeline->add_process(processm);

  sprokit::process::port_t const port_nameu = sprokit::process::port_t("shared");
  sprokit::process::port_t const port_named = sprokit::process::port_t("sink");
  sprokit::process::port_t const port_namem = sprokit::process::port_t("mutate");

  pipeline->connect(proc_nameu, port_nameu,
                    proc_named, port_named);

  EXPECT_EXCEPTION(sprokit::connection_flag_mismatch_exception,
                   pipeline->connect(proc_nameu, port_nameu,
                                     proc_namem, port_namem),
                   "connecting a mutate port to a shared port already connected to a port");
}

IMPLEMENT_TEST(connect)
{
  sprokit::process::type_t const proc_typeu = sprokit::process::type_t("numbers");
  sprokit::process::type_t const proc_typed = sprokit::process::type_t("multiplication");

  sprokit::process::name_t const proc_nameu = sprokit::process::name_t("upstream");
  sprokit::process::name_t const proc_named = sprokit::process::name_t("downstream");

  sprokit::process_t const processu = create_process(proc_typeu, proc_nameu);
  sprokit::process_t const processd = create_process(proc_typed, proc_named);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(processu);
  pipeline->add_process(processd);

  sprokit::process::port_t const port_nameu = sprokit::process::port_t("number");
  sprokit::process::port_t const port_named = sprokit::process::port_t("factor1");

  pipeline->connect(proc_nameu, port_nameu,
                    proc_named, port_named);
}

IMPLEMENT_TEST(setup_pipeline_no_processes)
{
  sprokit::pipeline_t const pipeline = create_pipeline();

  EXPECT_EXCEPTION(sprokit::no_processes_exception,
                   pipeline->setup_pipeline(),
                   "setting up an empty pipeline");
}

IMPLEMENT_TEST(setup_pipeline_orphaned_process)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("orphan");

  sprokit::process::name_t const proc_name1 = sprokit::process::name_t("orphan1");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("orphan2");

  sprokit::process_t const process1 = create_process(proc_type, proc_name1);
  sprokit::process_t const process2 = create_process(proc_type, proc_name2);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process1);
  pipeline->add_process(process2);

  EXPECT_EXCEPTION(sprokit::orphaned_processes_exception,
                   pipeline->setup_pipeline(),
                   "setting up a pipeline with orphaned processes");
}

IMPLEMENT_TEST(setup_pipeline_type_force_flow_upstream)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("flow_dependent");
  sprokit::process::type_t const proc_type2 = sprokit::process::type_t("take_string");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("flow");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("take");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const process2 = create_process(proc_type2, proc_name2);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);

  sprokit::process::port_t const port_name = sprokit::process::port_t("output");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("string");

  pipeline->connect(proc_name, port_name,
                    proc_name2, port_name2);

  pipeline->setup_pipeline();
}

IMPLEMENT_TEST(setup_pipeline_type_force_flow_downstream)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("numbers");
  sprokit::process::type_t const proc_type2 = sprokit::process::type_t("flow_dependent");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("data");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("flow");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const process2 = create_process(proc_type2, proc_name2);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);

  sprokit::process::port_t const port_name = sprokit::process::port_t("number");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("input");

  pipeline->connect(proc_name, port_name,
                    proc_name2, port_name2);

  pipeline->setup_pipeline();
}

IMPLEMENT_TEST(setup_pipeline_type_force_cascade_up)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("flow_dependent");
  sprokit::process::type_t const proc_type2 = sprokit::process::type_t("take_string");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("flow");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("flow2");
  sprokit::process::name_t const proc_name3 = sprokit::process::name_t("take");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const process2 = create_process(proc_type, proc_name2);
  sprokit::process_t const process3 = create_process(proc_type2, proc_name3);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);
  pipeline->add_process(process3);

  sprokit::process::port_t const port_name = sprokit::process::port_t("output");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("input");
  sprokit::process::port_t const port_name3 = sprokit::process::port_t("string");

  pipeline->connect(proc_name, port_name,
                    proc_name2, port_name2);
  pipeline->connect(proc_name2, port_name,
                    proc_name3, port_name3);

  pipeline->setup_pipeline();

  sprokit::process::port_info_t const info = process->output_port_info(port_name);

  if (boost::starts_with(info->type, sprokit::process::type_flow_dependent))
  {
    TEST_ERROR("Dependent types were not propagated properly up the pipeline");
  }
}

IMPLEMENT_TEST(setup_pipeline_type_force_cascade_down)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("numbers");
  sprokit::process::type_t const proc_type2 = sprokit::process::type_t("flow_dependent");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("data");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("flow");
  sprokit::process::name_t const proc_name3 = sprokit::process::name_t("flow2");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const process2 = create_process(proc_type2, proc_name2);
  sprokit::process_t const process3 = create_process(proc_type2, proc_name3);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);
  pipeline->add_process(process3);

  sprokit::process::port_t const port_name = sprokit::process::port_t("number");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("input");
  sprokit::process::port_t const port_name3 = sprokit::process::port_t("output");

  pipeline->connect(proc_name2, port_name3,
                    proc_name3, port_name2);
  pipeline->connect(proc_name, port_name,
                    proc_name2, port_name2);

  pipeline->setup_pipeline();

  sprokit::process::port_info_t const info = process3->input_port_info(port_name2);

  if (boost::starts_with(info->type, sprokit::process::type_flow_dependent))
  {
    TEST_ERROR("Dependent types were not propagated properly down the pipeline");
  }
}

IMPLEMENT_TEST(setup_pipeline_type_force_cascade_both)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("flow_dependent");
  sprokit::process::type_t const proc_type2 = sprokit::process::type_t("take_string");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("flow");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("flow2");
  sprokit::process::name_t const proc_name3 = sprokit::process::name_t("flow3");
  sprokit::process::name_t const proc_name4 = sprokit::process::name_t("take");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const process2 = create_process(proc_type, proc_name2);
  sprokit::process_t const process3 = create_process(proc_type, proc_name3);
  sprokit::process_t const process4 = create_process(proc_type2, proc_name4);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);
  pipeline->add_process(process3);
  pipeline->add_process(process4);

  sprokit::process::port_t const port_name = sprokit::process::port_t("output");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("input");
  sprokit::process::port_t const port_name3 = sprokit::process::port_t("string");

  pipeline->connect(proc_name, port_name,
                    proc_name2, port_name2);
  pipeline->connect(proc_name2, port_name,
                    proc_name3, port_name2);
  pipeline->connect(proc_name2, port_name,
                    proc_name4, port_name3);

  pipeline->setup_pipeline();

  sprokit::process::port_info_t info;

  info = process->output_port_info(port_name);

  if (boost::starts_with(info->type, sprokit::process::type_flow_dependent))
  {
    TEST_ERROR("Dependent types were not propagated properly within the pipeline");
  }

  info = process3->input_port_info(port_name2);

  if (boost::starts_with(info->type, sprokit::process::type_flow_dependent))
  {
    TEST_ERROR("Dependent types were not propagated properly within the pipeline");
  }
}

IMPLEMENT_TEST(setup_pipeline_backwards_edge)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("feedback");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("feedback");

  sprokit::process_t const process = create_process(proc_type, proc_name);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);

  sprokit::process::port_t const port_name = sprokit::process::port_t("output");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("input");

  pipeline->connect(proc_name, port_name,
                    proc_name, port_name2);

  pipeline->setup_pipeline();
}

IMPLEMENT_TEST(setup_pipeline_not_a_dag)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("flow_dependent");
  sprokit::process::type_t const proc_type2 = sprokit::process::type_t("multiplication");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("flow");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("flow2");
  sprokit::process::name_t const proc_name3 = sprokit::process::name_t("mult");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const process2 = create_process(proc_type, proc_name2);
  sprokit::process_t const process3 = create_process(proc_type2, proc_name3);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);
  pipeline->add_process(process3);

  sprokit::process::port_t const port_name = sprokit::process::port_t("output");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("input");
  sprokit::process::port_t const port_name3 = sprokit::process::port_t("factor1");
  sprokit::process::port_t const port_name4 = sprokit::process::port_t("factor2");
  sprokit::process::port_t const port_name5 = sprokit::process::port_t("product");

  pipeline->connect(proc_name, port_name,
                    proc_name3, port_name3);
  pipeline->connect(proc_name2, port_name,
                    proc_name3, port_name4);
  pipeline->connect(proc_name3, port_name5,
                    proc_name, port_name2);
  pipeline->connect(proc_name3, port_name5,
                    proc_name2, port_name2);

  EXPECT_EXCEPTION(sprokit::not_a_dag_exception,
                   pipeline->setup_pipeline(),
                   "a cycle is in the pipeline graph");
}

IMPLEMENT_TEST(setup_pipeline_data_dependent_set)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("data_dependent");
  sprokit::process::type_t const proc_type2 = sprokit::process::type_t("flow_dependent");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("data");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("sink");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const process2 = create_process(proc_type2, proc_name2);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);

  sprokit::process::port_t const port_name = sprokit::process::port_t("output");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("input");

  pipeline->connect(proc_name, port_name,
                    proc_name2, port_name2);

  pipeline->setup_pipeline();

  sprokit::process::port_info_t const info = process2->input_port_info(port_name2);

  if (boost::starts_with(info->type, sprokit::process::type_flow_dependent))
  {
    TEST_ERROR("Dependent types were not propagated properly down the pipeline after initialization");
  }
}

IMPLEMENT_TEST(setup_pipeline_data_dependent_set_reject)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("data_dependent");
  sprokit::process::type_t const proc_type2 = sprokit::process::type_t("flow_dependent");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("data");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("sink");

  sprokit::config_t conf = sprokit::config::empty_config();

  conf->set_value("reject", "true");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const process2 = create_process(proc_type2, proc_name2, conf);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);

  sprokit::process::port_t const port_name = sprokit::process::port_t("output");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("input");

  pipeline->connect(proc_name, port_name,
                    proc_name2, port_name2);

  EXPECT_EXCEPTION(sprokit::connection_dependent_type_exception,
                   pipeline->setup_pipeline(),
                   "a data dependent type propagation gets rejected");
}

IMPLEMENT_TEST(setup_pipeline_data_dependent_set_cascade)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("data_dependent");
  sprokit::process::type_t const proc_type2 = sprokit::process::type_t("flow_dependent");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("data");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("flow");
  sprokit::process::name_t const proc_name3 = sprokit::process::name_t("flow2");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const process2 = create_process(proc_type2, proc_name2);
  sprokit::process_t const process3 = create_process(proc_type2, proc_name3);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);
  pipeline->add_process(process3);

  sprokit::process::port_t const port_name = sprokit::process::port_t("output");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("input");

  pipeline->connect(proc_name, port_name,
                    proc_name2, port_name2);
  pipeline->connect(proc_name2, port_name,
                    proc_name3, port_name2);

  pipeline->setup_pipeline();

  sprokit::process::port_info_t info;

  info = process2->input_port_info(port_name2);

  if (boost::starts_with(info->type, sprokit::process::type_flow_dependent))
  {
    TEST_ERROR("Dependent types were not propagated properly down the pipeline after initialization");
  }

  info = process3->input_port_info(port_name2);

  if (boost::starts_with(info->type, sprokit::process::type_flow_dependent))
  {
    TEST_ERROR("Dependent types were not propagated properly down the pipeline after initialization");
  }
}

IMPLEMENT_TEST(setup_pipeline_data_dependent_set_cascade_reject)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("data_dependent");
  sprokit::process::type_t const proc_type2 = sprokit::process::type_t("flow_dependent");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("data");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("flow");
  sprokit::process::name_t const proc_name3 = sprokit::process::name_t("flow_reject");

  sprokit::config_t conf = sprokit::config::empty_config();

  conf->set_value("reject", "true");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const process2 = create_process(proc_type2, proc_name2);
  sprokit::process_t const process3 = create_process(proc_type2, proc_name3, conf);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);
  pipeline->add_process(process3);

  sprokit::process::port_t const port_name = sprokit::process::port_t("output");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("input");

  pipeline->connect(proc_name, port_name,
                    proc_name2, port_name2);
  pipeline->connect(proc_name2, port_name,
                    proc_name3, port_name2);

  EXPECT_EXCEPTION(sprokit::connection_dependent_type_cascade_exception,
                   pipeline->setup_pipeline(),
                   "a data dependent type propagation gets rejected");
}

IMPLEMENT_TEST(setup_pipeline_type_force_flow_upstream_reject)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("flow_dependent");
  sprokit::process::type_t const proc_type2 = sprokit::process::type_t("take_string");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("flow");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("take");

  sprokit::config_t conf = sprokit::config::empty_config();

  conf->set_value("reject", "true");

  sprokit::process_t const process = create_process(proc_type, proc_name, conf);
  sprokit::process_t const process2 = create_process(proc_type2, proc_name2);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);

  sprokit::process::port_t const port_name = sprokit::process::port_t("output");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("string");

  pipeline->connect(proc_name, port_name,
                    proc_name2, port_name2);

  EXPECT_EXCEPTION(sprokit::connection_dependent_type_exception,
                   pipeline->setup_pipeline(),
                   "setting up a pipeline where an upstream dependent type that gets rejected");
}

IMPLEMENT_TEST(setup_pipeline_type_force_flow_downstream_reject)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("numbers");
  sprokit::process::type_t const proc_type2 = sprokit::process::type_t("flow_dependent");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("data");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("flow");

  sprokit::config_t conf = sprokit::config::empty_config();

  conf->set_value("reject", "true");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const process2 = create_process(proc_type2, proc_name2, conf);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);

  sprokit::process::port_t const port_name = sprokit::process::port_t("number");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("input");

  pipeline->connect(proc_name, port_name,
                    proc_name2, port_name2);

  EXPECT_EXCEPTION(sprokit::connection_dependent_type_exception,
                   pipeline->setup_pipeline(),
                   "setting up a pipeline with a downstream dependent type that gets rejected");
}

IMPLEMENT_TEST(setup_pipeline_type_force_cascade_reject)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("numbers");
  sprokit::process::type_t const proc_type2 = sprokit::process::type_t("flow_dependent");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("data");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("flow");
  sprokit::process::name_t const proc_name3 = sprokit::process::name_t("flow_reject");

  sprokit::config_t conf = sprokit::config::empty_config();

  conf->set_value("reject", "true");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const process2 = create_process(proc_type2, proc_name2);
  sprokit::process_t const process3 = create_process(proc_type2, proc_name3, conf);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);
  pipeline->add_process(process3);

  sprokit::process::port_t const port_name = sprokit::process::port_t("number");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("input");
  sprokit::process::port_t const port_name3 = sprokit::process::port_t("output");

  pipeline->connect(proc_name2, port_name3,
                    proc_name3, port_name2);

  pipeline->connect(proc_name, port_name,
                    proc_name2, port_name2);

  EXPECT_EXCEPTION(sprokit::connection_dependent_type_cascade_exception,
                   pipeline->setup_pipeline(),
                   "setting up a pipeline where a dependent type that gets rejected elsewhere");
}

IMPLEMENT_TEST(setup_pipeline_untyped_data_dependent)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("data_dependent");
  sprokit::process::type_t const proc_type2 = sprokit::process::type_t("flow_dependent");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("data");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("flow");

  sprokit::config_t conf = sprokit::config::empty_config();

  conf->set_value("set_on_configure", "false");

  sprokit::process_t const process = create_process(proc_type, proc_name, conf);
  sprokit::process_t const process2 = create_process(proc_type2, proc_name2);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);

  sprokit::process::port_t const port_name = sprokit::process::port_t("output");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("input");

  pipeline->connect(proc_name, port_name,
                    proc_name2, port_name2);

  EXPECT_EXCEPTION(sprokit::untyped_data_dependent_exception,
                   pipeline->setup_pipeline(),
                   "a connected, unresolved data-dependent port exists after initialization");
}

IMPLEMENT_TEST(setup_pipeline_untyped_connection)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("flow_dependent");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("up");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("down");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const process2 = create_process(proc_type, proc_name2);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);

  sprokit::process::port_t const port_name = sprokit::process::port_t("output");
  sprokit::process::port_t const port_name2 = sprokit::process::port_t("input");

  pipeline->connect(proc_name, port_name,
                    proc_name2, port_name2);

  EXPECT_EXCEPTION(sprokit::untyped_connection_exception,
                   pipeline->setup_pipeline(),
                   "an untyped connection exists in the pipeline");
}

IMPLEMENT_TEST(setup_pipeline_missing_required_input_connection)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("take_string");

  sprokit::process_t const process = create_process(proc_type, sprokit::process::name_t());

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);

  EXPECT_EXCEPTION(sprokit::missing_connection_exception,
                   pipeline->setup_pipeline(),
                   "setting up a pipeline with missing required input connections");
}

IMPLEMENT_TEST(setup_pipeline_missing_required_output_connection)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("numbers");

  sprokit::process_t const process = create_process(proc_type, sprokit::process::name_t());

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);

  EXPECT_EXCEPTION(sprokit::missing_connection_exception,
                   pipeline->setup_pipeline(),
                   "setting up a pipeline with missing required output connections");
}

IMPLEMENT_TEST(setup_pipeline_duplicate)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("orphan");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("orphan");

  sprokit::process_t const process = create_process(proc_type, proc_name);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);

  pipeline->setup_pipeline();

  EXPECT_EXCEPTION(sprokit::pipeline_duplicate_setup_exception,
                   pipeline->setup_pipeline(),
                   "setting up a pipeline multiple times");
}

IMPLEMENT_TEST(setup_pipeline_add_process)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("orphan");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("orphan");

  sprokit::process_t const process = create_process(proc_type, proc_name);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);

  pipeline->setup_pipeline();

  EXPECT_EXCEPTION(sprokit::add_after_setup_exception,
                   pipeline->add_process(process),
                   "adding a process after setup");
}

IMPLEMENT_TEST(setup_pipeline_connect)
{
  sprokit::process::type_t const proc_typeu = sprokit::process::type_t("numbers");
  sprokit::process::type_t const proc_typed = sprokit::process::type_t("sink");

  sprokit::process::name_t const proc_nameu = sprokit::process::name_t("number");
  sprokit::process::name_t const proc_named = sprokit::process::name_t("sink");

  sprokit::process_t const processu = create_process(proc_typeu, proc_nameu);
  sprokit::process_t const processd = create_process(proc_typed, proc_named);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(processu);
  pipeline->add_process(processd);

  sprokit::process::port_t const port_nameu = sprokit::process::port_t("number");
  sprokit::process::port_t const port_named = sprokit::process::port_t("sink");

  pipeline->connect(proc_nameu, port_nameu,
                    proc_named, port_named);

  pipeline->setup_pipeline();

  sprokit::process::port_t const iport_name = sprokit::process::port_t("status");
  sprokit::process::port_t const oport_name = sprokit::process::port_heartbeat;

  EXPECT_EXCEPTION(sprokit::connection_after_setup_exception,
                   pipeline->connect(proc_named, oport_name,
                                     proc_nameu, iport_name),
                   "making a connection after setup");
}

IMPLEMENT_TEST(setup_pipeline)
{
  sprokit::process::type_t const proc_typeu = sprokit::process::type_t("numbers");
  sprokit::process::type_t const proc_typed = sprokit::process::type_t("multiplication");
  sprokit::process::type_t const proc_typet = sprokit::process::type_t("print_number");

  sprokit::process::name_t const proc_nameu1 = sprokit::process::name_t("upstream1");
  sprokit::process::name_t const proc_nameu2 = sprokit::process::name_t("upstream2");
  sprokit::process::name_t const proc_named = sprokit::process::name_t("downstream");
  sprokit::process::name_t const proc_namet = sprokit::process::name_t("terminal");

  sprokit::config_t const configt = sprokit::config::empty_config();

  sprokit::config::key_t const output_key = sprokit::config::key_t("output");
  sprokit::config::value_t const output_path = sprokit::config::value_t("test-pipeline-setup_pipeline-print_number.txt");

  configt->set_value(output_key, output_path);

  sprokit::process_t const processu1 = create_process(proc_typeu, proc_nameu1);
  sprokit::process_t const processu2 = create_process(proc_typeu, proc_nameu2);
  sprokit::process_t const processd = create_process(proc_typed, proc_named);
  sprokit::process_t const processt = create_process(proc_typet, proc_namet, configt);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(processu1);
  pipeline->add_process(processu2);
  pipeline->add_process(processd);
  pipeline->add_process(processt);

  sprokit::process::port_t const port_nameu = sprokit::process::port_t("number");
  sprokit::process::port_t const port_named1 = sprokit::process::port_t("factor1");
  sprokit::process::port_t const port_named2 = sprokit::process::port_t("factor2");
  sprokit::process::port_t const port_namedo = sprokit::process::port_t("product");
  sprokit::process::port_t const port_namet = sprokit::process::port_t("number");

  pipeline->connect(proc_nameu1, port_nameu,
                    proc_named, port_named1);
  pipeline->connect(proc_nameu2, port_nameu,
                    proc_named, port_named2);
  pipeline->connect(proc_named, port_namedo,
                    proc_namet, port_namet);

  pipeline->setup_pipeline();
}

static sprokit::scheduler_t create_scheduler(sprokit::pipeline_t const& pipe);

IMPLEMENT_TEST(start_before_setup)
{
  sprokit::pipeline_t const pipeline = create_pipeline();
  sprokit::scheduler_t const scheduler = create_scheduler(pipeline);

  EXPECT_EXCEPTION(sprokit::pipeline_not_setup_exception,
                   scheduler->start(),
                   "starting a pipeline that has not been setup");
}

IMPLEMENT_TEST(start_unsuccessful_setup)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("orphan");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("orphan");
  sprokit::process::name_t const proc_name2 = sprokit::process::name_t("orphan2");

  sprokit::process_t const process = create_process(proc_type, proc_name);
  sprokit::process_t const process2 = create_process(proc_type, proc_name2);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);
  pipeline->add_process(process2);

  try
  {
    pipeline->setup_pipeline();
  }
  catch (sprokit::pipeline_exception const&)
  {
  }

  sprokit::scheduler_t const scheduler = create_scheduler(pipeline);

  EXPECT_EXCEPTION(sprokit::pipeline_not_ready_exception,
                   scheduler->start(),
                   "starting a pipeline that has not been successfully setup");
}

IMPLEMENT_TEST(start_and_stop)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("orphan");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("orphan");

  sprokit::process_t const process = create_process(proc_type, proc_name);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);

  pipeline->setup_pipeline();

  sprokit::scheduler_t const scheduler = create_scheduler(pipeline);

  scheduler->start();
  scheduler->stop();
}

IMPLEMENT_TEST(reset_while_running)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("orphan");

  sprokit::process::name_t const proc_name = sprokit::process::name_t("orphan");

  sprokit::process_t const process = create_process(proc_type, proc_name);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(process);

  pipeline->setup_pipeline();

  sprokit::scheduler_t const scheduler = create_scheduler(pipeline);

  scheduler->start();

  EXPECT_EXCEPTION(sprokit::reset_running_pipeline_exception,
                   pipeline->reset(),
                   "resetting a running pipeline");
}

IMPLEMENT_TEST(reset)
{
  sprokit::process::type_t const proc_typeu = sprokit::process::type_t("numbers");
  sprokit::process::type_t const proc_typed = sprokit::process::type_t("multiplication");
  sprokit::process::type_t const proc_typet = sprokit::process::type_t("print_number");

  sprokit::process::name_t const proc_nameu1 = sprokit::process::name_t("upstream1");
  sprokit::process::name_t const proc_nameu2 = sprokit::process::name_t("upstream2");
  sprokit::process::name_t const proc_named = sprokit::process::name_t("downstream");
  sprokit::process::name_t const proc_namet = sprokit::process::name_t("terminal");

  sprokit::config_t const configt = sprokit::config::empty_config();

  sprokit::config::key_t const output_key = sprokit::config::key_t("output");
  sprokit::config::value_t const output_path = sprokit::config::value_t("test-pipeline-setup_pipeline-print_number.txt");

  configt->set_value(output_key, output_path);

  sprokit::process_t const processu1 = create_process(proc_typeu, proc_nameu1);
  sprokit::process_t const processu2 = create_process(proc_typeu, proc_nameu2);
  sprokit::process_t const processd = create_process(proc_typed, proc_named);
  sprokit::process_t const processt = create_process(proc_typet, proc_namet, configt);

  sprokit::pipeline_t const pipeline = create_pipeline();

  pipeline->add_process(processu1);
  pipeline->add_process(processu2);
  pipeline->add_process(processd);
  pipeline->add_process(processt);

  sprokit::process::port_t const port_nameu = sprokit::process::port_t("number");
  sprokit::process::port_t const port_named1 = sprokit::process::port_t("factor1");
  sprokit::process::port_t const port_named2 = sprokit::process::port_t("factor2");
  sprokit::process::port_t const port_namedo = sprokit::process::port_t("product");
  sprokit::process::port_t const port_namet = sprokit::process::port_t("number");

  pipeline->connect(proc_nameu1, port_nameu,
                    proc_named, port_named1);
  pipeline->connect(proc_nameu2, port_nameu,
                    proc_named, port_named2);
  pipeline->connect(proc_named, port_namedo,
                    proc_namet, port_namet);

  pipeline->setup_pipeline();

  pipeline->reset();

  pipeline->setup_pipeline();
}

IMPLEMENT_TEST(remove_process)
{
  sprokit::process::type_t const typeu = sprokit::process::type_t("orphan");
  sprokit::process::type_t const typed = sprokit::process::type_t("sink");
  sprokit::process::name_t const nameu = sprokit::process::name_t("up");
  sprokit::process::name_t const named = sprokit::process::name_t("down");

  sprokit::pipeline_t const pipe = create_pipeline();
  sprokit::process_t const procu = create_process(typeu, nameu);
  sprokit::process_t const procd = create_process(typed, named);

  pipe->add_process(procu);
  pipe->add_process(procd);

  sprokit::process::port_t const portu = sprokit::process::port_heartbeat;
  sprokit::process::port_t const portd = sprokit::process::port_t("sink");

  pipe->connect(nameu, portu,
                named, portd);

  pipe->remove_process(named);

  EXPECT_EXCEPTION(sprokit::no_such_process_exception,
                   pipe->process_by_name(named),
                   "requesting a process after it has been removed");

  if (!pipe->connections_from_addr(nameu, portu).empty())
  {
    TEST_ERROR("A connection exists after one of the processes has been removed");
  }
}

IMPLEMENT_TEST(remove_process_after_setup)
{
  sprokit::process::type_t const type = sprokit::process::type_t("orphan");
  sprokit::process::name_t const name = sprokit::process::name_t("name");

  sprokit::pipeline_t const pipe = create_pipeline();
  sprokit::process_t const proc = create_process(type, name);

  pipe->add_process(proc);

  pipe->setup_pipeline();

  EXPECT_EXCEPTION(sprokit::remove_after_setup_exception,
                   pipe->remove_process(name),
                   "removing a process after the pipeline has been setup");
}

IMPLEMENT_TEST(disconnect)
{
  sprokit::process::type_t const typeu = sprokit::process::type_t("orphan");
  sprokit::process::type_t const typed = sprokit::process::type_t("sink");
  sprokit::process::name_t const nameu = sprokit::process::name_t("up");
  sprokit::process::name_t const named = sprokit::process::name_t("down");

  sprokit::pipeline_t const pipe = create_pipeline();
  sprokit::process_t const procu = create_process(typeu, nameu);
  sprokit::process_t const procd = create_process(typed, named);

  pipe->add_process(procu);
  pipe->add_process(procd);

  sprokit::process::port_t const portu = sprokit::process::port_heartbeat;
  sprokit::process::port_t const portd = sprokit::process::port_t("sink");

  pipe->connect(nameu, portu,
                named, portd);
  pipe->disconnect(nameu, portu,
                   named, portd);

  if (!pipe->connections_from_addr(nameu, portu).empty())
  {
    TEST_ERROR("A connection exists after being disconnected");
  }
}

IMPLEMENT_TEST(disconnect_after_setup)
{
  sprokit::process::type_t const typeu = sprokit::process::type_t("orphan");
  sprokit::process::type_t const typed = sprokit::process::type_t("sink");
  sprokit::process::name_t const nameu = sprokit::process::name_t("up");
  sprokit::process::name_t const named = sprokit::process::name_t("down");

  sprokit::pipeline_t const pipe = create_pipeline();
  sprokit::process_t const procu = create_process(typeu, nameu);
  sprokit::process_t const procd = create_process(typed, named);

  pipe->add_process(procu);
  pipe->add_process(procd);

  sprokit::process::port_t const portu = sprokit::process::port_heartbeat;
  sprokit::process::port_t const portd = sprokit::process::port_t("sink");

  pipe->connect(nameu, portu,
                named, portd);

  pipe->setup_pipeline();

  EXPECT_EXCEPTION(sprokit::disconnection_after_setup_exception,
                   pipe->disconnect(nameu, portu,
                                    named, portd),
                   "requesting a disconnect after the pipeline has been setup");
}

IMPLEMENT_TEST(reconfigure_before_setup)
{
  sprokit::process::type_t const proc_type = sprokit::process::type_t("orphan");
  sprokit::process::name_t const proc_name = sprokit::process::name_t("name");

  sprokit::process_t const expect = create_process(proc_type, proc_name);

  sprokit::pipeline_t const pipeline = boost::make_shared<sprokit::pipeline>(sprokit::config::empty_config());

  pipeline->add_process(expect);

  sprokit::config_t const conf = sprokit::config::empty_config();

  EXPECT_EXCEPTION(sprokit::reconfigure_before_setup_exception,
                   pipeline->reconfigure(conf),
                   "reconfiguring a pipeline before it was setup");
}

class check_reconfigure_process
  : public sprokit::process
{
  public:
    check_reconfigure_process(sprokit::config_t const& conf);
    ~check_reconfigure_process();

    static sprokit::config::key_t const config_should_reconfigure;
  protected:
    void _reconfigure(sprokit::config_t const& conf);
  private:
    bool m_should_reconfigure;
    bool m_did_reconfigure;
};

IMPLEMENT_TEST(reconfigure)
{
  sprokit::process::name_t const proc_name = sprokit::process::name_t("name");

  sprokit::config_t const conf = sprokit::config::empty_config();

  conf->set_value(check_reconfigure_process::config_should_reconfigure, "true");
  conf->set_value(sprokit::process::config_name, proc_name);

  sprokit::process_t const check = boost::make_shared<check_reconfigure_process>(conf);

  sprokit::pipeline_t const pipeline = boost::make_shared<sprokit::pipeline>(sprokit::config::empty_config());

  pipeline->add_process(check);
  pipeline->setup_pipeline();

  sprokit::config_t const new_conf = sprokit::config::empty_config();

  sprokit::config::key_t const key = sprokit::config::key_t("new_key");
  sprokit::config::value_t const value = sprokit::config::value_t("old_value");

  new_conf->set_value(proc_name + sprokit::config::block_sep + key, value);

  pipeline->reconfigure(new_conf);
}

class sample_cluster
  : public sprokit::process_cluster
{
  public:
    sample_cluster(sprokit::config_t const& conf);
    ~sample_cluster();

    void _add_process(name_t const& name_, type_t const& type_, sprokit::config_t const& config);
};

IMPLEMENT_TEST(reconfigure_only_top_level)
{
  sprokit::process_registry_t const reg = sprokit::process_registry::self();

  sprokit::process::type_t const proc_type = sprokit::process::type_t("check_reconfigure");

  reg->register_process(proc_type, sprokit::process_registry::description_t(), sprokit::create_process<check_reconfigure_process>);

  sprokit::process::name_t const proc_name = sprokit::process::name_t("name");

  sprokit::config_t const conf = sprokit::config::empty_config();

  conf->set_value(check_reconfigure_process::config_should_reconfigure, "false");
  conf->set_value(sprokit::process::config_name, proc_name);

  typedef boost::shared_ptr<sample_cluster> sample_cluster_t;

  sprokit::config_t const cluster_conf = sprokit::config::empty_config();

  sprokit::process::name_t const cluster_name = sprokit::process::name_t("cluster");

  conf->set_value(sprokit::process::config_name, cluster_name);

  sample_cluster_t const cluster = boost::make_shared<sample_cluster>(cluster_conf);

  cluster->_add_process(proc_name, proc_type, conf);

  sprokit::pipeline_t const pipeline = boost::make_shared<sprokit::pipeline>(sprokit::config::empty_config());

  pipeline->add_process(cluster);
  pipeline->setup_pipeline();

  sprokit::config_t const new_conf = sprokit::config::empty_config();

  sprokit::config::key_t const key = sprokit::config::key_t("new_key");
  sprokit::config::value_t const value = sprokit::config::value_t("old_value");

  sprokit::process::name_t const resolved_name = cluster_name + "/" + proc_name;

  new_conf->set_value(resolved_name + sprokit::config::block_sep + key, value);

  pipeline->reconfigure(new_conf);
}

sprokit::process_t
create_process(sprokit::process::type_t const& type, sprokit::process::name_t const& name, sprokit::config_t config)
{
  static bool const modules_loaded = (sprokit::load_known_modules(), true);
  static sprokit::process_registry_t const reg = sprokit::process_registry::self();

  (void)modules_loaded;

  return reg->create_process(type, name, config);
}

sprokit::pipeline_t
create_pipeline()
{
  return boost::make_shared<sprokit::pipeline>();
}

class dummy_scheduler
  : public sprokit::scheduler
{
  public:
    dummy_scheduler(sprokit::pipeline_t const& pipe, sprokit::config_t const& config);
    ~dummy_scheduler();

    void _start();
    void _wait();
    void _pause();
    void _resume();
    void _stop();
};

sprokit::scheduler_t
create_scheduler(sprokit::pipeline_t const& pipe)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  return boost::make_shared<dummy_scheduler>(pipe, config);
}

dummy_scheduler
::dummy_scheduler(sprokit::pipeline_t const& pipe, sprokit::config_t const& config)
  : sprokit::scheduler(pipe, config)
{
}

dummy_scheduler
::~dummy_scheduler()
{
  shutdown();
}

void
dummy_scheduler
::_start()
{
}

void
dummy_scheduler
::_wait()
{
}

void
dummy_scheduler
::_pause()
{
}

void
dummy_scheduler
::_resume()
{
}

void
dummy_scheduler
::_stop()
{
}

sprokit::config::key_t const check_reconfigure_process::config_should_reconfigure = sprokit::config::key_t("should_reconfigure");

check_reconfigure_process
::check_reconfigure_process(sprokit::config_t const& conf)
  : process(conf)
  , m_did_reconfigure(false)
{
  m_should_reconfigure = conf->get_value<bool>(config_should_reconfigure);
}

check_reconfigure_process
::~check_reconfigure_process()
{
  if (m_did_reconfigure != m_should_reconfigure)
  {
    TEST_ERROR("Did not get expected reconfigure behavior: "
               "Expected: " << m_should_reconfigure << " "
               "Actual  : " << m_did_reconfigure);
  }
}

void
check_reconfigure_process
::_reconfigure(sprokit::config_t const& /*conf*/)
{
  m_did_reconfigure = true;
}

sample_cluster
::sample_cluster(sprokit::config_t const& conf)
  : sprokit::process_cluster(conf)
{
}

sample_cluster
::~sample_cluster()
{
}

void
sample_cluster
::_add_process(name_t const& name_, type_t const& type_, sprokit::config_t const& config)
{
  add_process(name_, type_, config);
}