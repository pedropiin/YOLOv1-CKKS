- Generating Context 
    - Set Parameters
        - CCParams<CryptoContextCKKSRNS> parameters;
            - ScalingModSize
                - Don't know how to choose so will select 50
                as it is the value seen in the OpenFHE
                examples and in FHEResNet20
                - simple-ckks-bootstrapping recommended 59
                with FirstModSize = 60 as default. So I'll
                test both
            - BatchSize
                - Can be number of parking spots in image (15) or 
                only one for the whole image
                - In reality, it may be the number of pixels as we
                are working with only one image
            - ScalingTechnique 
                - FLEXIBLEAUTOEXT for a quick prototype
            - NumLargeDigits
                - Again, not sure but will select 2 as it seems
                to be the basic/standard value
            - SecretKeyDist
                - UNIFORM_TERNARY as it is included in the 
                homomorphic encryption standard according 
                to the OpenFHE library documentation
            - SecurityLevel
                - HEStd_128_classic for 128-bit security level
                as it is suffiient
            - MultiplicativeDepth
                - Analyze YOLO circuit
                - Following OpenFHE examples:
                    - level_budget = {4, 4}
                    - levels_after_bootstrap = 10
                    - depth = levels_after +
                    FHECKKSRNS::GetBootstrapDepth(level, secretKeyDist)

    - Generate Context 
        - CryptoContext<DCRTPoly> context;

    - Enable()
        - PKE
        - KEYSWITCH
        - LEVELEDSHE
        - ADVANCEDSHE
        - FHE

    - Generate Keys
        - KeyGen()
        - EvalMultKeyGen(keys.secretKey)
        - As there are multiple layers in the YOLO CNN, I need
        to serialize and adjust the following evaluations 
        for each layer/step:
            - EvalBootstrapSetup(level_budget)
            - EvalBoostrapKeyGen(keys.secretKey, batchSize)
            - EvalRotateKeyGen()
                - Adjust according to the layer/step about to 
                be executed
    - Output RingDimension

- Run YOLOv1