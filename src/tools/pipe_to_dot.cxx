/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <vistk/pipeline_util/export_dot.h>
#include <vistk/pipeline_util/pipe_bakery.h>

#include <vistk/pipeline/modules.h>
#include <vistk/pipeline/types.h>

#include <boost/filesystem/operations.hpp>
#include <boost/program_options.hpp>

#include <fstream>
#include <iostream>

namespace po = boost::program_options;

static po::options_description make_options();
static void usage(po::options_description const& options);

int main(int argc, char* argv[])
{
  vistk::load_known_modules();

  po::options_description const desc = make_options();

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    usage(desc);
  }

  if (!vm.count("input"))
  {
    std::cerr << "Error: input not set" << std::endl;
    usage(desc);
  }

  vistk::pipeline_t pipe;

  {
    std::istream* pistr;
    std::ifstream fin;

    vistk::path_t const ipath = vm["input"].as<vistk::path_t>();

    if (ipath.native() == vistk::path_t("-"))
    {
      pistr = &std::cin;
    }
    else
    {
      fin.open(ipath.native().c_str());

      if (!fin.good())
      {
        std::cerr << "Error: Unable to open input file" << std::endl;

        return 1;
      }

      pistr = &fin;
    }

    std::istream& istr = *pistr;

    /// \todo Include paths?

    pipe = vistk::bake_pipe(istr, boost::filesystem::current_path());
  }

  if (!pipe)
  {
    std::cerr << "Error: Unable to bake pipeline" << std::endl;

    return 1;
  }

  std::ostream* postr;
  std::ofstream fout;

  vistk::path_t const opath = vm["output"].as<vistk::path_t>();

  if (opath.native() == vistk::path_t("-"))
  {
    postr = &std::cout;
  }
  else
  {
    fout.open(opath.native().c_str());

    if (fout.bad())
    {
      std::cerr << "Error: Unable to open output file" << std::endl;

      return 1;
    }

    postr = &fout;
  }

  std::ostream& ostr = *postr;

  std::string const graph_name = vm["name"].as<std::string>();

  vistk::export_dot(ostr, pipe, graph_name);

  return 0;
}

po::options_description
make_options()
{
  po::options_description desc;

  desc.add_options()
    ("help,h", "output help message and quit")
    ("input,i", po::value<vistk::path_t>(), "input path")
    ("output,o", po::value<vistk::path_t>()->default_value("-"), "output path")
    ("include,I", po::value<vistk::paths_t>(), "configuration include path")
    ("name,n", po::value<std::string>()->default_value("(unnamed)"), "name of the graph")
  ;

  return desc;
}

void
usage(po::options_description const& options)
{
  std::cerr << options << std::endl;

  exit(1);
}
