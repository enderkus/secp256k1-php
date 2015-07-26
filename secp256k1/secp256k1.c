/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_secp256k1.h"

static int le_secp256k1_ctx;

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_context_create, 0)
ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_context_destroy, 0)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_context_clone, 0)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_context_randomize, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, seed32)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_ecdsa_verify, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, msg32)
ZEND_ARG_INFO(0, signature)
ZEND_ARG_INFO(0, publicKey)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_ecdsa_sign, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, msg32)
ZEND_ARG_INFO(0, secretKey)
ZEND_ARG_INFO(1, signature)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_ecdsa_sign_compact, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, msg32)
ZEND_ARG_INFO(0, secretKey)
ZEND_ARG_INFO(1, signature)
ZEND_ARG_INFO(1, recid)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_ecdsa_recover_compact, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, msg32)
ZEND_ARG_INFO(0, signature)
ZEND_ARG_INFO(0, recid)
ZEND_ARG_INFO(0, compressed)
ZEND_ARG_INFO(1, publicKey)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_ec_seckey_verify, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, secretKey)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_ec_pubkey_verify, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, publicKey)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_ec_pubkey_create, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, secretKey)
ZEND_ARG_INFO(0, compressed)
ZEND_ARG_INFO(1, publicKey)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_ec_pubkey_decompress, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, publicKeyIn)
ZEND_ARG_INFO(1, publicKeyOut)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_ec_privkey_import, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, privkey)
ZEND_ARG_INFO(0, compressed)
ZEND_ARG_INFO(1, seckey)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_ec_privkey_export, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, seckey)
ZEND_ARG_INFO(0, compressed)
ZEND_ARG_INFO(1, derkey)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_ec_privkey_tweak_add, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(1, seckey)
ZEND_ARG_INFO(0, tweak)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_ec_pubkey_tweak_add, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(1, publicKey)
ZEND_ARG_INFO(0, tweak)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_ec_privkey_tweak_mul, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(1, seckey)
ZEND_ARG_INFO(0, tweak)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO(arginfo_secp256k1_ec_pubkey_tweak_mul, 0)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(1, publicKey)
ZEND_ARG_INFO(0, publicKeyLength)
ZEND_ARG_INFO(0, tweak)
ZEND_END_ARG_INFO();

static int le_secp256k1_ctx;


/* {{{ resource_functions[]
 *
 * Every user visible function must have an entry in resource_functions[].
 */
const zend_function_entry secp256k1_functions[] = {
        PHP_FE(secp256k1_context_create,        arginfo_secp256k1_context_create)
        PHP_FE(secp256k1_context_destroy,       arginfo_secp256k1_context_destroy)
        PHP_FE(secp256k1_context_clone,         arginfo_secp256k1_context_clone)
        PHP_FE(secp256k1_context_randomize,     arginfo_secp256k1_context_randomize)
        PHP_FE(secp256k1_ecdsa_verify,          arginfo_secp256k1_ecdsa_verify)
        PHP_FE(secp256k1_ecdsa_sign,            arginfo_secp256k1_ecdsa_sign)
        PHP_FE(secp256k1_ecdsa_sign_compact,    arginfo_secp256k1_ecdsa_sign_compact)
        PHP_FE(secp256k1_ecdsa_recover_compact, arginfo_secp256k1_ecdsa_recover_compact)
        PHP_FE(secp256k1_ec_seckey_verify,      arginfo_secp256k1_ec_seckey_verify)
        PHP_FE(secp256k1_ec_pubkey_verify,      arginfo_secp256k1_ec_pubkey_verify)
        PHP_FE(secp256k1_ec_pubkey_create,      arginfo_secp256k1_ec_pubkey_create)
        PHP_FE(secp256k1_ec_pubkey_decompress,  arginfo_secp256k1_ec_pubkey_decompress)
        PHP_FE(secp256k1_ec_privkey_import,     arginfo_secp256k1_ec_privkey_import)
        PHP_FE(secp256k1_ec_privkey_export,     arginfo_secp256k1_ec_privkey_export)
        PHP_FE(secp256k1_ec_privkey_tweak_add,  arginfo_secp256k1_ec_privkey_tweak_add)
        PHP_FE(secp256k1_ec_privkey_tweak_mul,  arginfo_secp256k1_ec_privkey_tweak_mul)
        PHP_FE(secp256k1_ec_pubkey_tweak_add,   arginfo_secp256k1_ec_pubkey_tweak_add)
        PHP_FE(secp256k1_ec_pubkey_tweak_mul,   arginfo_secp256k1_ec_pubkey_tweak_mul)
        PHP_FE_END	/* Must be the last line in resource_functions[] */
};
/* }}} */

static void secp256k1_ctx_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    printf("DTOR: secp256k1\n");
    secp256k1_context_t *ctx = (secp256k1_context_t*) rsrc->ptr;
    if (ctx) {
        printf("has ctx\n");
        secp256k1_context_destroy(ctx);
    }
    printf("secp256k1 dtor: done\n\n");
}

PHP_MINIT_FUNCTION(secp256k1) {
    le_secp256k1_ctx = zend_register_list_destructors_ex(secp256k1_ctx_dtor, NULL, SECP256K1_CTX_RES_NAME, module_number);
    REGISTER_LONG_CONSTANT("SECP256K1_CONTEXT_VERIFY", SECP256K1_CONTEXT_VERIFY, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SECP256K1_CONTEXT_SIGN", SECP256K1_CONTEXT_SIGN, CONST_CS | CONST_PERSISTENT);
    ZEND_INIT_MODULE_GLOBALS(secp256k1, php_secp256k1_init_globals, NULL);
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(secp256k1) {
    return SUCCESS;
}

/* Remove if there's nothing to do at request start */
PHP_RINIT_FUNCTION(secp256k1) {
    return SUCCESS;
}

/* Remove if there's nothing to do at request end */
PHP_RSHUTDOWN_FUNCTION(secp256k1) {
    return SUCCESS;
}

PHP_MINFO_FUNCTION(secp256k1) {
    php_info_print_table_start();
    php_info_print_table_header(2, "secp256k1 support", "enabled");
    php_info_print_table_end();
}

zend_module_entry secp256k1_module_entry = {
        STANDARD_MODULE_HEADER,
        "secp256k1",
        secp256k1_functions,
        PHP_MINIT(secp256k1),
        PHP_MSHUTDOWN(secp256k1),
        PHP_RINIT(secp256k1), /* Replace with NULL if there's nothing to do at request start */
        PHP_RSHUTDOWN(secp256k1), /* Replace with NULL if there's nothing to do at request end */
        PHP_MINFO(secp256k1),
        PHP_SECP256K1_VERSION,
        STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_SECP256K1
ZEND_GET_MODULE(secp256k1)
#endif

/* Create a secp256k1 context */
/* {{{ proto resource secp256k1_context_create(int flags)
   Return a secp256k1 context initialized with the desired pregenerated tables.
   NB: This is the most expensive operation in the library */
PHP_FUNCTION(secp256k1_context_create)
{
    long flags;
    secp256k1_context_t * ctx;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &flags) == FAILURE) {
        RETURN_FALSE;
    }

    ctx = secp256k1_context_create(flags);
    ZEND_REGISTER_RESOURCE(return_value, ctx, le_secp256k1_ctx);
}
/* }}} */

PHP_FUNCTION(secp256k1_context_destroy)
{
    zval *zCtx;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zCtx) == FAILURE) {
        RETURN_FALSE;
    }
    printf("called destroy");
    zend_list_delete(Z_LVAL_P(zCtx));
    RETURN_TRUE;
}

PHP_FUNCTION(secp256k1_context_clone)
{
    zval *zCtx;
    secp256k1_context_t *ctx;
    secp256k1_context_t *newCtx;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zCtx) == FAILURE) {
        RETURN_FALSE;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);

    newCtx = secp256k1_context_clone(ctx);
    ZEND_REGISTER_RESOURCE(return_value, newCtx, le_secp256k1_ctx);
}

PHP_FUNCTION(secp256k1_context_randomize)
{
    zval *zCtx;
    unsigned char *seed32 = NULL;
    int result, seedlen = 0;
    secp256k1_context_t *ctx;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|s", &zCtx, &seed32, seedlen) == FAILURE) {
        RETURN_FALSE;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);

    result = secp256k1_context_randomize(ctx, seed32);
    RETURN_LONG(result);
}

/**
 * Verify an ECDSA signature.
 *
 * In:
 *  msg32: the 32-byte message hash being verified (cannot be NULL)
 *  sig: the signature being verified (cannot be NULL)
 *  pubkey: the public key to verify with (cannot be NULL)
 * Returns:
 *  1: correct signature
 *  0: incorrect signature
 * -1: invalid public key
 * -2: invalid signature
 */
PHP_FUNCTION(secp256k1_ecdsa_verify) {
    zval *zCtx;
    secp256k1_context_t *ctx;
    unsigned char *msg32, *sig, *pubkey;
    int msg32len, siglen, pubkeylen;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rsss", &zCtx, &msg32, &msg32len, &sig, &siglen, &pubkey, &pubkeylen) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);

    int result = secp256k1_ecdsa_verify(ctx, msg32, sig, siglen, pubkey, pubkeylen);

    RETURN_LONG(result);
}


/**
 * Create an ECDSA signature.
 *
 * In:
 *  msg32:  the 32-byte message hash being signed (cannot be NULL)
 *  seckey: pointer to a 32-byte secret key (cannot be NULL)
 *
 * Out:
 *  sig:    pointer to an array where the signature will be placed (cannot be NULL)
 *
 * Returns:
 *  1: signature created
 *  0: the nonce generation function failed, the private key was invalid, or there is not
 *     enough space in the signature.
 *
 * The sig always has an s value in the lower half of the range (From 0x1
 * to 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF5D576E7357A4501DDFE92F46681B20A0,
 * inclusive), unlike many other implementations.
 * With ECDSA a third-party can can forge a second distinct signature
 * of the same message given a single initial signature without knowing
 * the key by setting s to its additive inverse mod-order, 'flipping' the
 * sign of the random point R which is not included in the signature.
 * Since the forgery is of the same message this isn't universally
 * problematic, but in systems where message malleability or uniqueness
 * of signatures is important this can cause issues.  This forgery can be
 * blocked by all verifiers forcing signers to use a canonical form. The
 * lower-S form reduces the size of signatures slightly on average when
 * variable length encodings (such as DER) are used and is cheap to
 * verify, making it a good choice. Security of always using lower-S is
 * assured because anyone can trivially modify a signature after the
 * fact to enforce this property.  Adjusting it inside the signing
 * function avoids the need to re-serialize or have curve specific
 * constants outside of the library.  By always using a canonical form
 * even in applications where it isn't needed it becomes possible to
 * impose a requirement later if a need is discovered.
 * No other forms of ECDSA malleability are known and none seem likely,
 * but there is no formal proof that ECDSA, even with this additional
 * restriction, is free of other malleability.  Commonly used serialization
 * schemes will also accept various non-unique encodings, so care should
 * be taken when this property is required for an application.
 */
PHP_FUNCTION(secp256k1_ecdsa_sign) {

    secp256k1_context_t *ctx;
    zval *zCtx, *signature;
    int newsiglen = 72, seckeylen, msg32len;
    unsigned char *seckey, *msg32, newsig[newsiglen];
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rssz", &zCtx, &msg32, &msg32len, &seckey, &seckeylen, &signature) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);

    int result = secp256k1_ecdsa_sign(ctx, msg32, newsig, &newsiglen, seckey, NULL, NULL);
    if (result) {
        ZVAL_STRINGL(signature, newsig, newsiglen, 1);
    }

    RETURN_LONG(result);
}

PHP_FUNCTION(secp256k1_ecdsa_sign_compact)
{
    zval *zCtx, *zSig, *zRecId;
    unsigned char *seckey, *msg32, *newsig;
    int result, seckeylen, msg32len, newsiglen = 64, newrecid;
    secp256k1_context_t *ctx;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rsszz", &zCtx, &seckey, &seckeylen, &msg32, &msg32len, &zSig, &zRecId) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);

    newsig = emalloc(newsiglen);
    result = secp256k1_ecdsa_sign_compact(ctx, msg32, newsig, seckey, NULL, NULL, &newrecid);
    if (result) {
        ZVAL_STRINGL(zSig, newsig, newsiglen, 1);
        ZVAL_LONG(zRecId, newrecid);
    }

    RETURN_LONG(result);
}

PHP_FUNCTION(secp256k1_ecdsa_recover_compact)
{
    zval *zCtx, *zPubKey;
    unsigned char *msg32, *signature, *newpubkey;
    long recid;
    int result, msg32len, signatureLen, compressed, newpubkeylen;
    secp256k1_context_t *ctx;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rssllz", &zCtx, &msg32, &msg32len, &signature, &signatureLen, &recid, &compressed, &zPubKey) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);

    newpubkeylen = compressed ? 33 : 65;
    newpubkey = emalloc(newpubkeylen);
    result = secp256k1_ecdsa_recover_compact(ctx, msg32, signature, newpubkey, &newpubkeylen, compressed, recid);
    if (result) {
        ZVAL_STRINGL(zPubKey, newpubkey, newpubkeylen, 1);
    }

    RETURN_LONG(result);
}

PHP_FUNCTION(secp256k1_ec_seckey_verify)
{
    zval *zCtx;
    secp256k1_context_t *ctx;
    unsigned char *seckey;
    int result, seckeylen;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &zCtx, &seckey, &seckeylen) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);

    result = secp256k1_ec_seckey_verify(ctx, seckey);
    RETURN_LONG(result);
}

PHP_FUNCTION(secp256k1_ec_pubkey_verify)
{
    zval *zCtx;
    secp256k1_context_t *ctx;
    unsigned char *pubkey;
    int result, pubkeylen;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &zCtx, &pubkey, &pubkeylen) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);

    result = secp256k1_ec_pubkey_verify(ctx, pubkey, pubkeylen);
    RETURN_LONG(result);
}

/**
 * Compute the public key for a secret key.
 *
 * In:
 *  compressed: whether the computed public key should be compressed
 *  seckey:     pointer to a 32-byte private key (cannot be NULL)
 *
 * Out:
y *  pubkey:     pointer to a 33-byte (if compressed) or 65-byte (if uncompressed)
 *              area to store the public key (cannot be NULL)
 *
 * Returns:
 *  1: secret was valid, public key stored
 *  0: secret was invalid, try again.
 */
PHP_FUNCTION(secp256k1_ec_pubkey_create)
{
    zval *zCtx, *zPubKey;
    secp256k1_context_t *ctx;
    int result, seckeylen, compressed, newpubkeylen;
    unsigned char *seckey, *newpubkey;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rslz", &zCtx, &seckey, &seckeylen, &compressed, &zPubKey) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);

    newpubkeylen = compressed ? 33 : 65;
    newpubkey = emalloc(newpubkeylen);
    result = secp256k1_ec_pubkey_create(ctx, newpubkey, &newpubkeylen, seckey, compressed);
    if (result) {
        ZVAL_STRINGL(zPubKey, newpubkey, newpubkeylen, 1);
    }
    RETURN_LONG(result);
}

PHP_FUNCTION(secp256k1_ec_pubkey_decompress)
{
    zval *zCtx, *zPubKey;
    secp256k1_context_t *ctx;
    unsigned char *pubkey, *newpubkey;
    int result, pubkeylen;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rsz", &zCtx, &pubkey, &pubkeylen, &zPubKey) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);

    newpubkey = malloc(65);
    result = secp256k1_ec_pubkey_decompress(ctx, pubkey, newpubkey, &pubkeylen);
    if (result) {
        ZVAL_STRINGL(zPubKey, newpubkey, pubkeylen, 1);
    }
    RETURN_LONG(result)
}

PHP_FUNCTION(secp256k1_ec_privkey_import)
{
    zval *zCtx, *zSecKey;
    secp256k1_context_t *ctx;
    unsigned char *derkey, *newseckey = emalloc(32);
    int result, derkeylen;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rsz", &zCtx, &derkey, &derkeylen, &zSecKey) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);

    result = secp256k1_ec_privkey_import(ctx, newseckey, derkey, derkeylen);
    if (result) {
        ZVAL_STRINGL(zSecKey, newseckey, 32, 1);
    }

    RETURN_LONG(result);
}

PHP_FUNCTION(secp256k1_ec_privkey_export)
{
    zval *zCtx, *zDerKey;
    secp256k1_context_t *ctx;
    int result, seckeylen, compressed, newkeylen = 300;
    unsigned char *seckey, *newkey;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rslz", &zCtx, &seckey, &seckeylen, &compressed, &zDerKey) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);

    newkey = emalloc(newkeylen);
    result = secp256k1_ec_privkey_export(ctx, seckey, newkey, &newkeylen, compressed ? 1 : 0);
    if (result) {
        ZVAL_STRINGL(zDerKey, newkey, newkeylen, 1);
    }
    RETURN_LONG(result);
}

PHP_FUNCTION(secp256k1_ec_privkey_tweak_add)
{
    zval *zCtx, *zSecKey;
    secp256k1_context_t *ctx;
    unsigned char *newseckey, *tweak;
    int result, tweaklen;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzs", &zCtx, &zSecKey, &tweak, &tweaklen) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);
    newseckey = Z_STRVAL_P(zSecKey);
    result = secp256k1_ec_privkey_tweak_add(ctx, newseckey, tweak);
    if (result) {
        ZVAL_STRINGL(zSecKey, newseckey, 32, 1);
    }
    RETURN_LONG(result);
}

PHP_FUNCTION(secp256k1_ec_pubkey_tweak_add)
{
    zval *zCtx, *zPubKey;
    secp256k1_context_t *ctx;
    unsigned char *tweak, *newpubkey;
    int result, tweaklen, newpubkeylen;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzs", &zCtx, &zPubKey, &tweak, &tweaklen) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);

    newpubkey = Z_STRVAL_P(zPubKey);
    newpubkeylen = Z_STRLEN_P(zPubKey);
    result = secp256k1_ec_pubkey_tweak_add(ctx, newpubkey, newpubkeylen, tweak);
    if (result) {
        ZVAL_STRINGL(zPubKey, newpubkey, newpubkeylen, 0);
    }
    RETURN_LONG(result);
}

PHP_FUNCTION(secp256k1_ec_privkey_tweak_mul)
{
    zval *zCtx, *zSecKey;
    unsigned char *newseckey, *tweak;
    secp256k1_context_t *ctx;
    int result, tweaklen;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzs", &zCtx, &zSecKey, &tweak, &tweaklen) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);

    result = secp256k1_ec_privkey_tweak_mul(ctx, newseckey, tweak);
    if (result) {
        Z_STRVAL_P(zSecKey) = newseckey;
        Z_STRLEN_P(zSecKey) = 32;
    }
    RETURN_LONG(result);
}

PHP_FUNCTION(secp256k1_ec_pubkey_tweak_mul)
{
    zval *zCtx, *zPubKey;
    unsigned char *tweak, *newpubkey;
    secp256k1_context_t *ctx;
    int result, tweaklen, newpubkeylen;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzs", &zCtx, &zPubKey, &tweak, &tweaklen) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);

    newpubkey = Z_STRVAL_P(zPubKey);
    newpubkeylen = Z_STRLEN_P(zPubKey);
    result = secp256k1_ec_pubkey_tweak_mul(ctx, newpubkey, newpubkeylen, tweak);
    if (result) {
        ZVAL_STRINGL(zPubKey, newpubkey, newpubkeylen, 0);
    }
    RETURN_LONG(result);
}

 /*
PHP_FUNCTION()
{
    zval *zCtx;
    secp256k1_context_t *ctx;
    int result;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zCtx) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(ctx, secp256k1_context_t*, &zCtx, -1, SECP256K1_CTX_RES_NAME, le_secp256k1_ctx);
    result = secp256k1
    if (result) {
    }
    RETURN_LONG(result);
}
 */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */