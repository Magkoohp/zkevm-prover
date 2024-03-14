#ifndef PROVER_HPP
#define PROVER_HPP

#include <map>
#include <pthread.h>
#include <semaphore.h>
#include "goldilocks_base_field.hpp"
#include "input.hpp"
#include "rom.hpp"
#include "proof_fflonk.hpp"
#include "alt_bn128.hpp"
#include "groth16.hpp"
#include "binfile_utils.hpp"
#include "zkey_utils.hpp"
#include "prover_request.hpp"
#include "poseidon_goldilocks.hpp"
#include "executor/executor.hpp"
#include "sm/pols_generated/constant_pols.hpp"


#include "starkRecursiveF.hpp"
#include "starkpil/stark_info.hpp"
#include "starks.hpp"
#include "constant_pols_starks.hpp"
#include "fflonk_prover.hpp"
class Prover
{
    Goldilocks &fr;
    PoseidonGoldilocks &poseidon;
    Executor executor;

    StarkRecursiveF *starksRecursiveF;

    Starks *starkBatch;
    Starks *starkBatchC12a;
    Starks *starkBatchRecursive1;
    Starks *starkBatchRecursive2;
    Starks *starkBlobInner;
    Starks *starkBlobInnerCompression;
    Starks *starkBlobInnerRecursive1;
    Starks *starkBlobOuter;
    Starks *starkBlobOuterRecursive2;
    

    Fflonk::FflonkProver<AltBn128::Engine> *prover;
    std::unique_ptr<Groth16::Prover<AltBn128::Engine>> groth16Prover;
    std::unique_ptr<BinFileUtils::BinFile> zkey;
    std::unique_ptr<ZKeyUtils::Header> zkeyHeader;
    mpz_t altBbn128r;

public:
    unordered_map<string, ProverRequest *> requestsMap; // Map uuid -> ProveRequest pointer

    vector<ProverRequest *> pendingRequests;   // Queue of pending requests
    ProverRequest *pCurrentRequest;            // Request currently being processed by the prover thread in server mode
    vector<ProverRequest *> completedRequests; // Map uuid -> ProveRequest pointer

private:
    pthread_t proverPthread;  // Prover thread
    pthread_t cleanerPthread; // Garbage collector
    pthread_mutex_t mutex;    // Mutex to protect the requests queues
    void *pAddress = NULL;
    void *pAddressStarksRecursiveF = NULL;
    int protocolId;
    uint64_t polsSize;
    bool optimizeMemoryNTT = false;
    bool optimizeMemoryNTTCommitPols = false;
public:
    const Config &config;
    sem_t pendingRequestSem; // Semaphore to wakeup prover thread when a new request is available
    string lastComputedRequestId;
    uint64_t lastComputedRequestEndTime;

    Prover(Goldilocks &fr,
           PoseidonGoldilocks &poseidon,
           const Config &config);

    ~Prover();

    void genBatchProof(ProverRequest *pProverRequest);
    void genBlobInnerProof(ProverRequest *pProverRequest){
            zklog.info("Prover::genBlobInnerProof() timestamp: " + pProverRequest->timestamp);
            zklog.info("Prover::genBlobInnerProof() UUID: " + pProverRequest->uuid);
            zklog.info("Prover::genBlobInnerProof() input file: " + pProverRequest->inputFile());
    };
    void genBlobOuterProof(ProverRequest *pProverRequest){
            zklog.info("Prover::genBlobOuterProof() timestamp: " + pProverRequest->timestamp);
            zklog.info("Prover::genBlobOuterProof() UUID: " + pProverRequest->uuid);
            zklog.info("Prover::genBlobOuterProof() input file: " + pProverRequest->inputFile());
    };
    void genAggregatedBatchProof(ProverRequest *pProverRequest) ;
    void genAggregatedBlobOuterProof(ProverRequest *pProverRequest){
            zklog.info("Prover::genAggregatedBlobOuterProof() timestamp: " + pProverRequest->timestamp);
            zklog.info("Prover::genAggregatedBlobOuterProof() UUID: " + pProverRequest->uuid);
            zklog.info("Prover::genAggregatedBlobOuterProof() input file: " + pProverRequest->inputFile());
    };
    void genFinalProof(ProverRequest *pProverRequest);
    void processBatch(ProverRequest *pProverRequest);
    void executeBatch(ProverRequest *pProverRequest);
    void processBlobInner(ProverRequest *pProverRequest);
    void executeBlobInner(ProverRequest *pProverRequest);
    
    string submitRequest(ProverRequest *pProverRequest);                                          // returns UUID for this request
    ProverRequest *waitForRequestToComplete(const string &uuid, const uint64_t timeoutInSeconds); // wait for the request with this UUID to complete; returns NULL if UUID is invalid

    void lock(void) { pthread_mutex_lock(&mutex); };
    void unlock(void) { pthread_mutex_unlock(&mutex); };
};

void *proverThread(void *arg);
void *cleanerThread(void *arg);

#endif