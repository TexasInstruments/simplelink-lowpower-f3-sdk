/* psa_internal_trusted_storage.c
 *
 * Dummy implementation of the PSA internal trusted storage APIs.
 *
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
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/psa/internal_trusted_storage.h> // the API to implement


/*----------------------------------------------------------------------------
 * Definitions and macros
 */

#define MAX_ITS_BUFF_SIZE 10U

typedef struct psa_its_context_s
{
    struct psa_storage_info_t info;
    uint8_t data[776+16];
    uint8_t allocated;
} psa_its_context_t;

static psa_its_context_t its_buff[MAX_ITS_BUFF_SIZE] = { 0 };


/*----------------------------------------------------------------------------
 * psa_its_get_info
 */
psa_status_t
psa_its_get_info(psa_storage_uid_t uid,
                 struct psa_storage_info_t * p_info)
{
    psa_status_t funcres = PSA_SUCCESS;

    if ((0U == uid) || (MAX_ITS_BUFF_SIZE <= uid))
    {
        funcres = PSA_ERROR_INVALID_HANDLE;
    }
    else if (0U != its_buff[uid].allocated)
    {
        *p_info = its_buff[uid].info;
    }
    else
    {
        funcres = PSA_ERROR_DOES_NOT_EXIST;
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psa_its_set
 */
psa_status_t
psa_its_set(psa_storage_uid_t uid,
            size_t data_length,
            const void * p_data,
            psa_storage_create_flags_t create_flags)
{
    psa_status_t funcres = PSA_SUCCESS;

    if ((0U == uid) || (MAX_ITS_BUFF_SIZE <= uid))
    {
        funcres = PSA_ERROR_INVALID_HANDLE;
    }
    else if (0U != its_buff[uid].allocated)
    {
        funcres = PSA_ERROR_ALREADY_EXISTS;
    }
    else if (data_length > sizeof(its_buff[0].data))
    {
        funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
    }
    else
    {
        its_buff[uid].allocated = 255U;
        (void)memcpy((void *)its_buff[uid].data,
                     (const void *)p_data, data_length);
        its_buff[uid].info.size = data_length;
        its_buff[uid].info.flags = create_flags;
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psa_its_get
 */
psa_status_t
psa_its_get(psa_storage_uid_t uid,
            size_t data_offset,
            size_t data_size,
            void * p_data,
            size_t * p_data_length)
{
    psa_status_t funcres = PSA_SUCCESS;
    (void)data_offset;

    if ((0U == uid) || (MAX_ITS_BUFF_SIZE <= uid))
    {
        funcres = PSA_ERROR_INVALID_HANDLE;
    }
    else if (0U != its_buff[uid].allocated)
    {
        if (data_size >= its_buff[uid].info.size)
        {
            (void)memcpy((void *)p_data,
                         (const void *)its_buff[uid].data,
                         its_buff[uid].info.size);
            *p_data_length = its_buff[uid].info.size;
        }
        else
        {
            funcres = PSA_ERROR_BUFFER_TOO_SMALL;
        }
    }
    else
    {
        funcres = PSA_ERROR_DOES_NOT_EXIST;
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psa_its_remove
 */
psa_status_t
psa_its_remove(psa_storage_uid_t uid)
{
    psa_status_t funcres = PSA_SUCCESS;

    if ((0U == uid) || (MAX_ITS_BUFF_SIZE <= uid))
    {
        funcres = PSA_ERROR_DOES_NOT_EXIST;
    }
    else
    {
        (void)memset(its_buff[uid].data, 0, sizeof(its_buff[uid].data));
        its_buff[uid].info.capacity = 0U;
        its_buff[uid].info.size = 0U;
        its_buff[uid].info.flags = 0U;
        its_buff[uid].allocated = 0U;
    }

    return funcres;
}


/* end of file psa_internal_trusted_storage.c */
