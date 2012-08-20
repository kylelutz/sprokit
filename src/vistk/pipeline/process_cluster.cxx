/*ckwg +5
 * Copyright 2012 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "process_cluster.h"

#include "pipeline_exception.h"
#include "process_exception.h"
#include "process_registry.h"

#include <boost/foreach.hpp>

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

/**
 * \file process_cluster.cxx
 *
 * \brief Implementation for \link vistk::process_cluster process cluster\endlink.
 */

namespace vistk
{

process::property_t const process_cluster::property_cluster = process::property_t("_cluster");

class process_cluster::priv
{
  public:
    priv();
    ~priv();

    typedef std::pair<config::key_t, config::key_t> config_mapping_t;
    typedef std::vector<config_mapping_t> config_mappings_t;
    typedef std::map<process::name_t, config_mappings_t> config_map_t;

    void ensure_name(name_t const& name) const;

    config_map_t config_map;
    names_t names;
    processes_t processes;
    connections_t input_mappings;
    connections_t output_mappings;
    connections_t internal_connections;
};

processes_t
process_cluster
::processes() const
{
  return d->processes;
}

process::connections_t
process_cluster
::input_mappings() const
{
  return d->input_mappings;
}

process::connections_t
process_cluster
::output_mappings() const
{
  return d->output_mappings;
}

process::connections_t
process_cluster
::internal_connections() const
{
  return d->internal_connections;
}

process_cluster
::process_cluster(config_t const& config)
  : process(config)
  , d(new priv)
{
}

process_cluster
::~process_cluster()
{
}

static process::name_t convert_name(process::name_t const& cluster_name, process::name_t const& process_name);

void
process_cluster
::map_config(config::key_t const& key, name_t const& name_, config::key_t const& mapped_key)
{
  priv::config_mapping_t const mapping = priv::config_mapping_t(key, mapped_key);

  d->config_map[name_].push_back(mapping);
}

void
process_cluster
::add_process(name_t const& name_, type_t const& type_, config_t const& conf)
{
  names_t::const_iterator const i = std::find(d->names.begin(), d->names.end(), name_);

  if (i != d->names.end())
  {
    throw duplicate_process_name_exception(name_);
  }

  priv::config_mappings_t const mappings = d->config_map[name_];

  BOOST_FOREACH (priv::config_mapping_t const& mapping, mappings)
  {
    config::key_t const& key = mapping.first;
    config::key_t const& mapped_key = mapping.second;

    config::value_t const value = config_value<config::value_t>(key);

    conf->set_value(mapped_key, value);
  }

  process_registry_t const reg = process_registry::self();
  name_t const real_name = convert_name(name(), name_);

  process_t const proc = reg->create_process(type_, real_name, conf);

  d->names.push_back(name_);
  d->processes.push_back(proc);
}

void
process_cluster
::input_map(port_t const& port, name_t const& name_, port_t const& mapped_port)
{
  d->ensure_name(name_);

  name_t const real_name = convert_name(name(), name_);

  port_addr_t const cluster_addr = port_addr_t(name(), port);
  port_addr_t const mapped_addr = port_addr_t(real_name, mapped_port);

  connection_t const connection = connection_t(cluster_addr, mapped_addr);

  d->input_mappings.push_back(connection);
}

void
process_cluster
::output_map(port_t const& port, name_t const& name_, port_t const& mapped_port)
{
  d->ensure_name(name_);

  name_t const real_name = convert_name(name(), name_);

  port_addr_t const cluster_addr = port_addr_t(name(), port);
  port_addr_t const mapped_addr = port_addr_t(real_name, mapped_port);

  connection_t const connection = connection_t(mapped_addr, cluster_addr);

  d->output_mappings.push_back(connection);
}

void
process_cluster
::connect(name_t const& upstream_name, port_t const& upstream_port,
          name_t const& downstream_name, port_t const& downstream_port)
{
  d->ensure_name(upstream_name);
  d->ensure_name(downstream_name);

  name_t const up_real_name = convert_name(name(), upstream_name);
  name_t const down_real_name = convert_name(name(), downstream_name);

  port_addr_t const up_addr = port_addr_t(up_real_name, upstream_port);
  port_addr_t const down_addr = port_addr_t(down_real_name, downstream_port);

  connection_t const connection = connection_t(up_addr, down_addr);

  d->internal_connections.push_back(connection);
}

void
process_cluster
::_configure()
{
}

void
process_cluster
::_init()
{
}

void
process_cluster
::_reset()
{
}

void
process_cluster
::_step()
{
  throw process_exception();
}

process::properties_t
process_cluster
::_properties() const
{
  properties_t base_properties = process::_properties();

  base_properties.insert(property_cluster);

  return base_properties;
}

process_cluster::priv
::priv()
{
}

process_cluster::priv
::~priv()
{
}

void
process_cluster::priv
::ensure_name(name_t const& name) const
{
  names_t::const_iterator const i = std::find(names.begin(), names.end(), name);

  if (i == names.end())
  {
    throw no_such_process_exception(name);
  }
}

process::name_t
convert_name(process::name_t const& cluster_name, process::name_t const& process_name)
{
  static process::name_t const sep = process::name_t("/");

  process::name_t const full_name = cluster_name + sep + process_name;

  return full_name;
}

}