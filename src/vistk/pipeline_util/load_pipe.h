/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef VISTK_PIPELINE_UTIL_LOAD_PIPE_H
#define VISTK_PIPELINE_UTIL_LOAD_PIPE_H

#include "pipeline_util-config.h"

#include <vistk/pipeline/types.h>

#include <boost/filesystem/path.hpp>

#include <istream>

/**
 * \file load_pipe.h
 *
 * \brief Load a pipeline declaration from a stream.
 */

namespace vistk
{

/**
 * \brief Convert a pipeline description file into a pipeline.
 *
 * \param fname The file to load the pipeline from
 *
 * \returns A new pipeline.
 */
pipeline_t VISTK_PIPELINE_UTIL_EXPORT load_pipe_file(boost::filesystem::path const& fname);

/**
 * \brief Convert a pipeline description into a pipeline.
 *
 * \param istr The stream to load the pipeline from.
 *
 * \returns A new pipeline.
 */
pipeline_t VISTK_PIPELINE_UTIL_EXPORT load_pipe(std::istream& istr);

}

#endif // VISTK_PIPELINE_UTIL_LOAD_PIPE_H