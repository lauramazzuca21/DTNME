/*
 *    Copyright 2004-2006 Intel Corporation
 * 
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *        http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/*
 *    Modifications made to this file by the patch file dtnme_mfs-33289-1.patch
 *    are Copyright 2015 United States Government as represented by NASA
 *       Marshall Space Flight Center. All Rights Reserved.
 *
 *    Released under the NASA Open Source Software Agreement version 1.3;
 *    You may obtain a copy of the Agreement at:
 * 
 *        http://ti.arc.nasa.gov/opensource/nosa/
 * 
 *    The subject software is provided "AS IS" WITHOUT ANY WARRANTY of any kind,
 *    either expressed, implied or statutory and this agreement does not,
 *    in any manner, constitute an endorsement by government agency of any
 *    results, designs or products resulting from use of the subject software.
 *    See the Agreement for the specific language governing permissions and
 *    limitations.
 */

#ifdef HAVE_CONFIG_H
#  include <dtn-config.h>
#endif

#include <oasys/util/ScratchBuffer.h>

#include "PingRegistration.h"
#include "RegistrationTable.h"
#include "bundling/BundleDaemon.h"

namespace dtn {

PingRegistration::PingRegistration(const EndpointID& eid)
    : Registration(PING_REGID, eid, Registration::DEFER, Registration::NONE, 0, 0) 
{
    logpathf("/dtn/reg/ping");
    set_active(true);
}

void
PingRegistration::deliver_bundle(Bundle* bundle)
{
    size_t payload_len = bundle->payload().length();
    log_debug("%zu byte ping from %s",
              payload_len, bundle->source().c_str());
    
    Bundle* reply = new Bundle();
    reply->mutable_source()->assign(endpoint_);
    reply->mutable_dest()->assign(bundle->source());
    reply->mutable_replyto()->assign(EndpointID::NULL_EID());
    reply->mutable_custodian()->assign(EndpointID::NULL_EID());
    reply->set_expiration(bundle->expiration());

    reply->mutable_payload()->set_length(payload_len);
    reply->mutable_payload()->write_data(bundle->payload(), 0, payload_len, 0);

    BundleDaemon::post(new BundleReceivedEvent(reply, EVENTSRC_ADMIN));
    // mark bundle as delivered
    bundle->fwdlog()->update(this, ForwardingInfo::DELIVERED);
    BundleDaemon::post(new BundleDeliveredEvent(bundle, this));
}


} // namespace dtn
