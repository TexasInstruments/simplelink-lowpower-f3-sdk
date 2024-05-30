/* adapter_psa_key_derivation.c
 *
 * Implementation of the PSA API.
 *
 * This file implements the key derivation services.
 */

/* -------------------------------------------------------------------------- */
/*                                                                            */
/*   Module        : DDK-130_bsd                                              */
/*   Version       : 4.1.1                                                    */
/*   Configuration : DDK_EIP130_BSD                                           */
/*                                                                            */
/*   Date          : 2023-Mar-10                                              */
/*                                                                            */
/* Copyright (c) 2014-2023 by Rambus, Inc. and/or its subsidiaries.           */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or without         */
/* modification, are permitted provided that the following conditions are     */
/* met:                                                                       */
/*                                                                            */
/* 1. Redistributions of source code must retain the above copyright          */
/* notice, this list of conditions and the following disclaimer.              */
/*                                                                            */
/* 2. Redistributions in binary form must reproduce the above copyright       */
/* notice, this list of conditions and the following disclaimer in the        */
/* documentation and/or other materials provided with the distribution.       */
/*                                                                            */
/* 3. Neither the name of the copyright holder nor the names of its           */
/* contributors may be used to endorse or promote products derived from       */
/* this software without specific prior written permission.                   */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR      */
/* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT       */
/* HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT           */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,      */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY      */
/* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT        */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE      */
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.       */
/* -------------------------------------------------------------------------- */

#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/c_adapter_psa.h>              // configuration
#include <third_party/hsmddk/include/Kit/DriverFramework/Basic_Defs_API/incl/basic_defs.h>
#include <third_party/hsmddk/include/Kit/Log/incl/log.h>
#include <third_party/hsmddk/include/Kit/DriverFramework/CLib_Abstraction_API/incl/clib.h>                       // size_t
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/psa/crypto.h>                 // the API to implement
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_internal.h>

/*----------------------------------------------------------------------------
 * Definitions and macros
 */


/*----------------------------------------------------------------------------
 * psa_key_derivation_setup
 */
psa_status_t
psa_key_derivation_setup(psa_key_derivation_operation_t * operation,
                         psa_algorithm_t alg)
{
    (void)operation;
    (void)alg;

    return PSA_ERROR_NOT_SUPPORTED;
}


/*----------------------------------------------------------------------------
 * psa_key_derivation_get_capacity
 */
psa_status_t
psa_key_derivation_get_capacity(const psa_key_derivation_operation_t *operation,
                                size_t *capacity)
{
    (void)operation;
    (void)capacity;

    return PSA_ERROR_NOT_SUPPORTED;
}


/*----------------------------------------------------------------------------
 * psa_key_derivation_set_capacity
 */
psa_status_t
psa_key_derivation_set_capacity(psa_key_derivation_operation_t * operation,
                                size_t capacity)
{
    (void)operation;
    (void)capacity;

    return PSA_ERROR_NOT_SUPPORTED;
}


/*----------------------------------------------------------------------------
 * psa_key_derivation_input_bytes
 */
psa_status_t
psa_key_derivation_input_bytes(psa_key_derivation_operation_t * operation,
                               psa_key_derivation_step_t step,
                               const uint8_t * data,
                               size_t data_length)
{
    (void)operation;
    (void)step;
    (void)data;
    (void)data_length;

    return PSA_ERROR_NOT_SUPPORTED;
}


/*----------------------------------------------------------------------------
 * psa_key_derivation_input_integer
 */
psa_status_t
psa_key_derivation_input_integer(psa_key_derivation_operation_t * operation,
                                 psa_key_derivation_step_t step,
                                 uint64_t value)
{
    (void)operation;
    (void)step;
    (void)value;

    return PSA_ERROR_NOT_SUPPORTED;
}


/*----------------------------------------------------------------------------
 * psa_key_derivation_input_key
 */
psa_status_t
psa_key_derivation_input_key(psa_key_derivation_operation_t * operation,
                             psa_key_derivation_step_t step,
                             psa_key_id_t key)
{
    (void)operation;
    (void)step;
    (void)key;

    return PSA_ERROR_NOT_SUPPORTED;
}


/*----------------------------------------------------------------------------
 * psa_key_derivation_output_bytes
 */
psa_status_t
psa_key_derivation_output_bytes(psa_key_derivation_operation_t * operation,
                                uint8_t * output,
                                size_t output_length)
{
    (void)operation;
    (void)output;
    (void)output_length;

    return PSA_ERROR_NOT_SUPPORTED;
}


/*----------------------------------------------------------------------------
 * psa_key_derivation_output_key
 */
psa_status_t
psa_key_derivation_output_key(const psa_key_attributes_t * attributes,
                              psa_key_derivation_operation_t * operation,
                              psa_key_id_t * key)
{
    (void)attributes;
    (void)operation;
    (void)key;

    return PSA_ERROR_NOT_SUPPORTED;
}


/*----------------------------------------------------------------------------
 * psa_key_derivation_verify_bytes
 */
psa_status_t
psa_key_derivation_verify_bytes(psa_key_derivation_operation_t * operation,
                                const uint8_t *expected_output,
                                size_t output_length)
{
    (void)operation;
    (void)expected_output;
    (void)output_length;

    return PSA_ERROR_NOT_SUPPORTED;
}


/*----------------------------------------------------------------------------
 * psa_key_derivation_verify_key
 */
psa_status_t
psa_key_derivation_verify_key(psa_key_derivation_operation_t * operation,
                              psa_key_id_t expected)
{
    (void)operation;
    (void)expected;

    return PSA_ERROR_NOT_SUPPORTED;
}


/*----------------------------------------------------------------------------
 * psa_key_derivation_set_capacity
 */
psa_status_t
psa_key_derivation_abort(psa_key_derivation_operation_t * operation)
{
    (void)operation;

    return PSA_ERROR_NOT_SUPPORTED;
}


/*----------------------------------------------------------------------------
 * psa_key_derivation_key_agreement
 */
psa_status_t
psa_key_derivation_key_agreement(psa_key_derivation_operation_t * operation,
                                 psa_key_derivation_step_t step,
                                 psa_key_id_t private_key,
                                 const uint8_t * peer_key,
                                 size_t peer_key_length)
{
    (void)operation;
    (void)step;
    (void)private_key;
    (void)peer_key;
    (void)peer_key_length;

    return PSA_ERROR_NOT_SUPPORTED;
}


/* end of file adapter_psa_key_derivation.c */