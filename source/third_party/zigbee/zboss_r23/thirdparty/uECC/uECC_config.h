/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifndef UECC_CONFIG_H
#define UECC_CONFIG_H

/* Skip everything not realted to public key generation */
#define UECC_KEY_GEN_ONLY 0

/**
 * Default word size for arm is 4, but thus we end up using 64-bit variables;
 * size 2 is not supported, so we manually enable size 1
 */
#define uECC_WORD_SIZE 4

#define uECC_ASM uECC_asm_none

#define uECC_CURVE uECC_secp256r1

/**
 * uECC_SQUARE_FUNC - If enabled (defined as nonzero), this will cause a specific function to be
 * used for (scalar) squaring instead of the generic multiplication function. This will make things
 * faster by about 8% but increases the code size.
 */
#define uECC_SQUARE_FUNC 1

/**
 * There is a feature inside uECC, that provides additional security, as it says, but it leads 
 * to generating different secrets and keys, so better not to use it 
 * before careful investigation of what it is
 */
#define uECC_USE_REGULARIZATION 0

#endif /* UECC_CONFIG_H */
