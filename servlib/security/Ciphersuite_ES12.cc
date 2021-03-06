/*
 *    Copyright 2006-7 SPARTA Inc
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

#ifdef HAVE_CONFIG_H
#  include <dtn-config.h>
#endif

#ifdef BSP_ENABLED

#include "Ciphersuite_ES12.h"
#include "ECDH.h"

namespace dtn {

static const char * log = "/dtn/bundle/ciphersuite";

//----------------------------------------------------------------------
Ciphersuite_ES12::Ciphersuite_ES12()
{
}

//----------------------------------------------------------------------
u_int16_t
Ciphersuite_ES12::cs_num(void)
{
    return CSNUM_ES;
}

//----------------------------------------------------------------------
int Ciphersuite_ES12::decrypt(
                              std::string   security_dest,
                              const LocalBuffer&   enc_data,          // ECDH-encrypted Bundle Encryption Key (BEK)
                              LocalBuffer&   result)		       // decrypted BEK will go here
{

    std::string priv_key_file = Ciphersuite::config->get_priv_key_dec(security_dest, cs_num());

    if(0 != ECDH::decrypt(result, priv_key_file.c_str(), enc_data)) {
        log_err_p(log, "Ciphersuite_ES12::decrypt: decryption failed");
        return -1;
    } else {
        log_debug_p(log, "Ciphersuite_ES12::decrypt: decryption succeeded");
    }


    return 0;
}

//----------------------------------------------------------------------
int Ciphersuite_ES12::encrypt(
							  std::string       security_dest,
							  const LocalBuffer&       data,      // the Bundle Encryption Key (BEK) to be encrypted
							  LocalBuffer&       result)        // encrypted BEK will go here
{
	(void)link;

	std::string certfile;

	certfile = Ciphersuite::config->get_pub_key_enc(security_dest,cs_num());

	return ECDH::encrypt(data, certfile.c_str(), result, 192);
}

} // namespace dtn

#endif /* BSP_ENABLED */
