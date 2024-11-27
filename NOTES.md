# Notes

### Image and Batch Size
- Original image from camera: 884x884x3 ==> 2344368 pixels/slots
- Original input size from YOLOv1: 448x448x3 ==> 602112 pixels/slots
- Reduced image 1: 221x221x3 ==> 146523 pixels/slots
- Reduced image 2: 110x110x3 ==> 36300 pixels/slots
- Reduced image 3 (MNIST dimension): 32x32x3 ==> 3072 pixels/slots
- Reduction 1 to close power of 2: 104x104x3 ==> 32448 pixels/slots
- Reductino 2 to close power of 2: 147x147x3 ==> 64827 pixels/slots

### Parameter Configuration
- The batch size needs to be a power of 2 and smaller than
the ring_dimension / 2. So, reducing images to 221x221 
won't work. The closests powers of 2 we have to half the 
ring dimension are 2¹5 = 32768 and 2¹6 = 65536.
Therefore, we have two options:
    - Reduce images to 104x104, so that it takes 32448
    slots and fits in a batch_size of 1 << 15;
    - Reduce images to 147x147, so that it takes 64827 
    slots and fits in a batch size of 1 << 16

- Program was only able to complete BootstrapKeyGen() when the
level budget was {5, 5}, image was 104x104 and Ring Dimension
set to 1 << 16 (2^16).
    - Setting SecurityLevel to "HEStd_128_classic" crashed
    - Setting level budget to {4, 4} crashed
    - 

### Tests
- Using images both in 104x104x3 and 147x147x3 format isn't working,
because the program crashes during the generation of the Boostrapping
keys. 
    - Tested with ScalingModSize = 59 and FirstModSize = 60 && 
    ScalingModSize = 47 and both FirstModSize = 52 and 45.