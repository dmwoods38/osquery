/**
 *  Copyright (c) 2014-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed as defined on the LICENSE file found in the
 *  root directory of this source tree.
 */

#include <osquery/logger.h>
#include <osquery/sql.h>
#include <osquery/system.h>
#include <osquery/tables.h>

#include <osquery/utils/conversions/tryto.h>
#include "osquery/core/windows/wmi.h"

namespace osquery {
namespace tables {

QueryData genDiskInfo(QueryContext& context) {
  Row r;
  QueryData results;

  const WmiRequest wmiSystemReq("select * from Win32_DiskDrive");
  const std::vector<WmiResultItem>& wmiResults = wmiSystemReq.results();
  if (wmiResults.empty()) {
    LOG(WARNING) << "Error retrieving information from WMI.";
    return results;
  }
  for (const auto& data : wmiResults) {
    long partitionCount = 0;
    long index = 0;
    data.GetLong("Partitions", partitionCount);
    r["partitions"] = INTEGER(partitionCount);
    data.GetLong("Index", index);
    r["disk_index"] = INTEGER(index);
    data.GetString("InterfaceType", r["type"]);
    data.GetString("PNPDeviceID", r["pnp_device_id"]);
    data.GetString("DeviceID", r["id"]);
    data.GetString("Size", r["disk_size"]);
    data.GetString("Manufacturer", r["manufacturer"]);
    data.GetString("Model", r["hardware_model"]);
    data.GetString("Name", r["name"]);
    data.GetString("SerialNumber", r["serial"]);
    data.GetString("Description", r["description"]);
    results.push_back(r);
  }

  return results;
}
} // namespace tables
} // namespace osquery
