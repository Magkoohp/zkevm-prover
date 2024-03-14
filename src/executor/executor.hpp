#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "config.hpp"
#include "goldilocks_base_field.hpp"
#include "main_sm/fork_1/main/main_executor.hpp"
#include "main_sm/fork_2/main/main_executor.hpp"
#include "main_sm/fork_3/main/main_executor.hpp"
#include "main_sm/fork_4/main/main_executor.hpp"
#include "main_sm/fork_5/main/main_executor.hpp"
#include "main_sm/fork_6/main/main_executor.hpp"
#include "main_sm/fork_7/main/main_executor.hpp"
#include "main_sm/fork_8/main/main_executor.hpp"
#include "main_sm/fork_8/main_exec_c/main_exec_c.hpp"
#include "main_sm/fork_9/main/main_executor.hpp"
#include "main_sm/fork_9_blob/main/main_executor.hpp"
#include "sm/storage/storage_executor.hpp"
#include "sm/memory/memory_executor.hpp"
#include "sm/binary/binary_executor.hpp"
#include "sm/arith/arith_executor.hpp"
#include "sm/padding_kk/padding_kk_executor.hpp"
#include "sm/padding_kkbit/padding_kkbit_executor.hpp"
#include "sm/bits2field/bits2field_executor.hpp"
#include "sm/keccak_f/keccak_f_executor.hpp"
#include "sm/padding_pg/padding_pg_executor.hpp"
#include "sm/poseidon_g/poseidon_g_executor.hpp"
#include "sm/mem_align/mem_align_executor.hpp"
#include "sm/padding_sha256/padding_sha256_executor.hpp"
#include "sm/padding_sha256bit/padding_sha256bit_executor.hpp"
#include "sm/sha256_f/sha256_f_executor.hpp"
#include "sm/climb_key/climb_key_executor.hpp"
#include "prover_request.hpp"

class Executor
{
public:
    Goldilocks &fr;
    const Config &config;

    fork_1::MainExecutor mainExecutor_fork_1;
    fork_2::MainExecutor mainExecutor_fork_2;
    fork_3::MainExecutor mainExecutor_fork_3;
    fork_4::MainExecutor mainExecutor_fork_4;
    fork_5::MainExecutor mainExecutor_fork_5;
    fork_6::MainExecutor mainExecutor_fork_6;
    fork_7::MainExecutor mainExecutor_fork_7;
    fork_8::MainExecutor mainExecutor_fork_8;
    fork_8::MainExecutorC mainExecutorC_fork_8;
    fork_9::MainExecutor mainExecutor_fork_9;
    fork_9_blob::MainExecutor mainExecutor_fork_9_blob;
    StorageExecutor storageExecutor;
    MemoryExecutor memoryExecutor;
    BinaryExecutor binaryExecutor;
    ArithExecutor arithExecutor;
    PaddingKKExecutor paddingKKExecutor;
    PaddingKKBitExecutor paddingKKBitExecutor;
    Bits2FieldExecutor bits2FieldExecutor;
    KeccakFExecutor keccakFExecutor;
    PaddingSha256Executor paddingSha256Executor;
    PaddingSha256BitExecutor paddingSha256BitExecutor;
    Bits2FieldSha256Executor bits2FieldSha256Executor;
    Sha256FExecutor sha256FExecutor;
    PaddingPGExecutor paddingPGExecutor;
    PoseidonGExecutor poseidonGExecutor;
    MemAlignExecutor memAlignExecutor;
    ClimbKeyExecutor climbKeyExecutor;


    Executor(Goldilocks &fr, const Config &config, PoseidonGoldilocks &poseidon) :
        fr(fr),
        config(config),
        mainExecutor_fork_1(fr, poseidon, config),
        mainExecutor_fork_2(fr, poseidon, config),
        mainExecutor_fork_3(fr, poseidon, config),
        mainExecutor_fork_4(fr, poseidon, config),
        mainExecutor_fork_5(fr, poseidon, config),
        mainExecutor_fork_6(fr, poseidon, config),
        mainExecutor_fork_7(fr, poseidon, config),
        mainExecutor_fork_8(fr, poseidon, config),
        mainExecutorC_fork_8(mainExecutor_fork_8),
        mainExecutor_fork_9(fr, poseidon, config),
        mainExecutor_fork_9_blob(fr, poseidon, config),
        storageExecutor(fr, poseidon, config),
        memoryExecutor(fr, config),
        binaryExecutor(fr, config),
        arithExecutor(fr, config),
        paddingKKExecutor(fr),
        paddingKKBitExecutor(fr),
        bits2FieldExecutor(fr),
        keccakFExecutor(fr, config),
        paddingSha256Executor(fr),
        paddingSha256BitExecutor(fr),
        bits2FieldSha256Executor(fr),
        sha256FExecutor(fr, config),
        paddingPGExecutor(fr, poseidon),
        poseidonGExecutor(fr, poseidon),
        memAlignExecutor(fr, config),
        climbKeyExecutor(fr, config)
        {};

    // Full version: all polynomials are evaluated, in all evaluations
    void executeBatch (ProverRequest &proverRequest, PROVER_FORK_NAMESPACE::CommitPols & commitPols);

    // Reduced version: only 2 evaluations are allocated, and assert is disabled
    void processBatch (ProverRequest &proverRequest);

    // Full version: all polynomials are evaluated, in all evaluations
    void executeBlobInner (ProverRequest &proverRequest, PROVER_BLOB_FORK_NAMESPACE::CommitPols & commitPols);

    // Reduced version: only 2 evaluations are allocated, and assert is disabled
    void processBlobInner (ProverRequest &proverRequest);
};

#endif