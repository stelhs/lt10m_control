#include "thread_table.h"

__attribute__((section(".rodata"), aligned(4)))
const struct thread_metric thread_metric_table[] = {
   {
        .diameter = 6,
        .step = 1000,
        .is_default = 1,
        .bolt_major = {5794, 5974},
        .bolt_minor = {4596, 4891},
        .internal_major = {4917, 5153},
        .internal_minor = {6000, 6294},
        .drill_size = 5000
   },
   {
        .diameter = 6,
        .step = 800,
        .is_default = 0,
        .bolt_major = {5826, 5976},
        .bolt_minor = {4884, 5110},
        .internal_major = {5134, 5334},
        .internal_minor = {6000, 6221},
        .drill_size = 5200
   },
   {
        .diameter = 6,
        .step = 750,
        .is_default = 0,
        .bolt_major = {5838, 5978},
        .bolt_minor = {4929, 5166},
        .internal_major = {5188, 5378},
        .internal_minor = {6000, 6240},
        .drill_size = 5250
   },
   {
        .diameter = 6,
        .step = 700,
        .is_default = 0,
        .bolt_major = {5838, 5978},
        .bolt_minor = {4997, 5220},
        .internal_major = {5242, 5422},
        .internal_minor = {6000, 6226},
        .drill_size = 5300
   },
   {
        .diameter = 6,
        .step = 500,
        .is_default = 0,
        .bolt_major = {5874, 5980},
        .bolt_minor = {5262, 5439},
        .internal_major = {5459, 5599},
        .internal_minor = {6000, 6184},
        .drill_size = 5500
   },
   {
        .diameter = 7,
        .step = 1000,
        .is_default = 1,
        .bolt_major = {6794, 6974},
        .bolt_minor = {5596, 5891},
        .internal_major = {5917, 6153},
        .internal_minor = {7000, 7294},
        .drill_size = 6000
   },
   {
        .diameter = 7,
        .step = 750,
        .is_default = 0,
        .bolt_major = {6838, 6978},
        .bolt_minor = {5929, 6166},
        .internal_major = {6188, 6378},
        .internal_minor = {7000, 7240},
        .drill_size = 6250
   },
   {
        .diameter = 7,
        .step = 500,
        .is_default = 0,
        .bolt_major = {6874, 6980},
        .bolt_minor = {6262, 6439},
        .internal_major = {6459, 6599},
        .internal_minor = {7000, 7184},
        .drill_size = 6500
   },
   {
        .diameter = 8,
        .step = 1250,
        .is_default = 1,
        .bolt_major = {7760, 7972},
        .bolt_minor = {6272, 6619},
        .internal_major = {6647, 6912},
        .internal_minor = {8000, 8340},
        .drill_size = 6750
   },
   {
        .diameter = 8,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {7794, 7974},
        .bolt_minor = {6596, 6891},
        .internal_major = {6917, 7153},
        .internal_minor = {8000, 8294},
        .drill_size = 7000
   },
   {
        .diameter = 8,
        .step = 800,
        .is_default = 0,
        .bolt_major = {7826, 7976},
        .bolt_minor = {6858, 7110},
        .internal_major = {7134, 7334},
        .internal_minor = {8000, 8255},
        .drill_size = 7200
   },
   {
        .diameter = 8,
        .step = 750,
        .is_default = 0,
        .bolt_major = {7838, 7978},
        .bolt_minor = {6929, 7166},
        .internal_major = {7188, 7378},
        .internal_minor = {8000, 8240},
        .drill_size = 7250
   },
   {
        .diameter = 8,
        .step = 500,
        .is_default = 0,
        .bolt_major = {7874, 7980},
        .bolt_minor = {7262, 7439},
        .internal_major = {7459, 7599},
        .internal_minor = {8000, 8183},
        .drill_size = 7500
   },
   {
        .diameter = 9,
        .step = 1250,
        .is_default = 0,
        .bolt_major = {8760, 8972},
        .bolt_minor = {7272, 7619},
        .internal_major = {7647, 7912},
        .internal_minor = {9000, 9340},
        .drill_size = 7250
   },
   {
        .diameter = 9,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {8794, 8974},
        .bolt_minor = {7596, 7891},
        .internal_major = {7917, 8153},
        .internal_minor = {9000, 9292},
        .drill_size = 8000
   },
   {
        .diameter = 9,
        .step = 750,
        .is_default = 0,
        .bolt_major = {8838, 8978},
        .bolt_minor = {7929, 8166},
        .internal_major = {8188, 8378},
        .internal_minor = {9000, 9240},
        .drill_size = 8250
   },
   {
        .diameter = 9,
        .step = 500,
        .is_default = 0,
        .bolt_major = {8874, 8980},
        .bolt_minor = {8262, 8439},
        .internal_major = {8459, 8599},
        .internal_minor = {9000, 9184},
        .drill_size = 8500
   },
   {
        .diameter = 10,
        .step = 1500,
        .is_default = 1,
        .bolt_major = {9732, 9968},
        .bolt_minor = {7938, 8344},
        .internal_major = {8376, 8676},
        .internal_minor = {10000, 10396},
        .drill_size = 8500
   },
   {
        .diameter = 10,
        .step = 1250,
        .is_default = 0,
        .bolt_major = {9760, 9972},
        .bolt_minor = {8272, 8619},
        .internal_major = {8647, 8912},
        .internal_minor = {10000, 10340},
        .drill_size = 8750
   },
   {
        .diameter = 10,
        .step = 1120,
        .is_default = 0,
        .bolt_major = {9783, 9973},
        .bolt_minor = {8438, 8761},
        .internal_major = {8788, 9038},
        .internal_minor = {10000, 10322},
        .drill_size = 8880
   },
   {
        .diameter = 10,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {9794, 9974},
        .bolt_minor = {8596, 8891},
        .internal_major = {8917, 9153},
        .internal_minor = {10000, 10294},
        .drill_size = 9000
   },
   {
        .diameter = 10,
        .step = 750,
        .is_default = 0,
        .bolt_major = {9838, 9978},
        .bolt_minor = {8929, 9166},
        .internal_major = {9188, 9378},
        .internal_minor = {10000, 10240},
        .drill_size = 9250
   },
   {
        .diameter = 10,
        .step = 500,
        .is_default = 0,
        .bolt_major = {9874, 9980},
        .bolt_minor = {9262, 9439},
        .internal_major = {9459, 9599},
        .internal_minor = {10000, 10184},
        .drill_size = 9500
   },
   {
        .diameter = 11,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {10730, 10970},
        .bolt_minor = {8938, 9344},
        .internal_major = {9376, 9676},
        .internal_minor = {11000, 11387},
        .drill_size = 9500
   },
   {
        .diameter = 11,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {10790, 10970},
        .bolt_minor = {9596, 9891},
        .internal_major = {9917, 10153},
        .internal_minor = {11000, 11294},
        .drill_size = 10000
   },
   {
        .diameter = 11,
        .step = 750,
        .is_default = 0,
        .bolt_major = {10840, 10980},
        .bolt_minor = {9929, 10166},
        .internal_major = {10188, 10378},
        .internal_minor = {11000, 11240},
        .drill_size = 10250
   },
   {
        .diameter = 11,
        .step = 500,
        .is_default = 0,
        .bolt_major = {10870, 10980},
        .bolt_minor = {10262, 10439},
        .internal_major = {10459, 10599},
        .internal_minor = {11000, 11184},
        .drill_size = 10500
   },
   {
        .diameter = 12,
        .step = 1750,
        .is_default = 1,
        .bolt_major = {11700, 11970},
        .bolt_minor = {9601, 10072},
        .internal_major = {10106, 10441},
        .internal_minor = {12000, 12453},
        .drill_size = 10250
   },
   {
        .diameter = 12,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {11730, 11970},
        .bolt_minor = {9930, 10344},
        .internal_major = {10376, 10676},
        .internal_minor = {12000, 12406},
        .drill_size = 10500
   },
   {
        .diameter = 12,
        .step = 1250,
        .is_default = 0,
        .bolt_major = {11760, 11970},
        .bolt_minor = {10258, 10619},
        .internal_major = {10647, 10912},
        .internal_minor = {12000, 12360},
        .drill_size = 10750
   },
   {
        .diameter = 12,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {11790, 11970},
        .bolt_minor = {10590, 10891},
        .internal_major = {10917, 11153},
        .internal_minor = {12000, 12304},
        .drill_size = 11000
   },
   {
        .diameter = 12,
        .step = 750,
        .is_default = 0,
        .bolt_major = {11840, 11980},
        .bolt_minor = {10923, 11166},
        .internal_major = {11188, 11378},
        .internal_minor = {12000, 12248},
        .drill_size = 11250
   },
   {
        .diameter = 12,
        .step = 500,
        .is_default = 0,
        .bolt_major = {11870, 13980},
        .bolt_minor = {11257, 11439},
        .internal_major = {11459, 11599},
        .internal_minor = {12000, 12192},
        .drill_size = 11500
   },
   {
        .diameter = 14,
        .step = 2000,
        .is_default = 1,
        .bolt_major = {13680, 13960},
        .bolt_minor = {11271, 11797},
        .internal_major = {11835, 12210},
        .internal_minor = {14000, 14501},
        .drill_size = 12000
   },
   {
        .diameter = 14,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {13730, 13970},
        .bolt_minor = {11930, 12344},
        .internal_major = {12376, 12676},
        .internal_minor = {14000, 14406},
        .drill_size = 12500
   },
   {
        .diameter = 14,
        .step = 1250,
        .is_default = 0,
        .bolt_major = {13760, 13970},
        .bolt_minor = {12258, 12619},
        .internal_major = {12647, 12912},
        .internal_minor = {14000, 14350},
        .drill_size = 12750
   },
   {
        .diameter = 14,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {13790, 13970},
        .bolt_minor = {12590, 12891},
        .internal_major = {12917, 13153},
        .internal_minor = {14000, 14304},
        .drill_size = 13000
   },
   {
        .diameter = 14,
        .step = 750,
        .is_default = 0,
        .bolt_major = {13840, 13980},
        .bolt_minor = {12923, 13166},
        .internal_major = {13188, 13378},
        .internal_minor = {14000, 14248},
        .drill_size = 13250
   },
   {
        .diameter = 14,
        .step = 500,
        .is_default = 0,
        .bolt_major = {13870, 13980},
        .bolt_minor = {13257, 13439},
        .internal_major = {13459, 13599},
        .internal_minor = {14000, 14192},
        .drill_size = 13500
   },
   {
        .diameter = 15,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {14730, 14970},
        .bolt_minor = {12930, 13344},
        .internal_major = {13376, 13676},
        .internal_minor = {15000, 15407},
        .drill_size = 13500
   },
   {
        .diameter = 15,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {14790, 14970},
        .bolt_minor = {13590, 13891},
        .internal_major = {13917, 14153},
        .internal_minor = {15000, 15304},
        .drill_size = 14000
   },
   {
        .diameter = 16,
        .step = 2000,
        .is_default = 1,
        .bolt_major = {15680, 15960},
        .bolt_minor = {13271, 13797},
        .internal_major = {13835, 14210},
        .internal_minor = {16000, 16501},
        .drill_size = 14000
   },
   {
        .diameter = 16,
        .step = 1600,
        .is_default = 0,
        .bolt_major = {15760, 15970},
        .bolt_minor = {13838, 14236},
        .internal_major = {14268, 14568},
        .internal_minor = {16000, 16370},
        .drill_size = 14400
   },
   {
        .diameter = 16,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {15730, 15970},
        .bolt_minor = {13930, 14344},
        .internal_major = {14376, 14676},
        .internal_minor = {16000, 16406},
        .drill_size = 14500
   },
   {
        .diameter = 16,
        .step = 1250,
        .is_default = 0,
        .bolt_major = {15760, 15970},
        .bolt_minor = {14258, 14619},
        .internal_major = {14647, 14912},
        .internal_minor = {16000, 16350},
        .drill_size = 14750
   },
   {
        .diameter = 16,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {15790, 15970},
        .bolt_minor = {14590, 14891},
        .internal_major = {14917, 15153},
        .internal_minor = {16000, 16303},
        .drill_size = 15000
   },
   {
        .diameter = 16,
        .step = 750,
        .is_default = 0,
        .bolt_major = {15840, 15980},
        .bolt_minor = {14923, 15166},
        .internal_major = {15188, 15378},
        .internal_minor = {16000, 16248},
        .drill_size = 15250
   },
   {
        .diameter = 16,
        .step = 500,
        .is_default = 0,
        .bolt_major = {15870, 15980},
        .bolt_minor = {15257, 15439},
        .internal_major = {15459, 15599},
        .internal_minor = {16000, 16192},
        .drill_size = 15500
   },
   {
        .diameter = 17,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {16730, 16970},
        .bolt_minor = {14930, 15344},
        .internal_major = {15376, 15676},
        .internal_minor = {17000, 17407},
        .drill_size = 15500
   },
   {
        .diameter = 17,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {16790, 16970},
        .bolt_minor = {15590, 15891},
        .internal_major = {15917, 16152},
        .internal_minor = {17000, 17304},
        .drill_size = 16000
   },
   {
        .diameter = 18,
        .step = 2500,
        .is_default = 1,
        .bolt_major = {17620, 17960},
        .bolt_minor = {14624, 15252},
        .internal_major = {15294, 15744},
        .internal_minor = {18000, 18585},
        .drill_size = 15500
   },
   {
        .diameter = 18,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {17680, 17960},
        .bolt_minor = {15271, 15797},
        .internal_major = {15835, 16210},
        .internal_minor = {18000, 18501},
        .drill_size = 16000
   },
   {
        .diameter = 18,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {17730, 17970},
        .bolt_minor = {15930, 16344},
        .internal_major = {16376, 16676},
        .internal_minor = {18000, 18406},
        .drill_size = 16500
   },
   {
        .diameter = 18,
        .step = 1250,
        .is_default = 0,
        .bolt_major = {17760, 17970},
        .bolt_minor = {16258, 16619},
        .internal_major = {16647, 16912},
        .internal_minor = {18000, 18350},
        .drill_size = 16750
   },
   {
        .diameter = 18,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {17790, 17970},
        .bolt_minor = {16590, 16891},
        .internal_major = {16917, 17153},
        .internal_minor = {18000, 18304},
        .drill_size = 17000
   },
   {
        .diameter = 18,
        .step = 750,
        .is_default = 0,
        .bolt_major = {17840, 17980},
        .bolt_minor = {16923, 17166},
        .internal_major = {17188, 17378},
        .internal_minor = {18000, 18248},
        .drill_size = 17250
   },
   {
        .diameter = 18,
        .step = 500,
        .is_default = 0,
        .bolt_major = {17870, 17980},
        .bolt_minor = {17257, 17439},
        .internal_major = {17459, 17599},
        .internal_minor = {18000, 18192},
        .drill_size = 17500
   },
   {
        .diameter = 20,
        .step = 2500,
        .is_default = 1,
        .bolt_major = {19620, 19960},
        .bolt_minor = {16624, 17252},
        .internal_major = {17294, 17744},
        .internal_minor = {20000, 20585},
        .drill_size = 17500
   },
   {
        .diameter = 20,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {19680, 19960},
        .bolt_minor = {17271, 17797},
        .internal_major = {17835, 18210},
        .internal_minor = {20000, 20501},
        .drill_size = 18000
   },
   {
        .diameter = 20,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {19730, 19970},
        .bolt_minor = {17930, 18344},
        .internal_major = {18376, 18676},
        .internal_minor = {20000, 20406},
        .drill_size = 18500
   },
   {
        .diameter = 20,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {19790, 19970},
        .bolt_minor = {18590, 18891},
        .internal_major = {18917, 19153},
        .internal_minor = {20000, 20304},
        .drill_size = 19000
   },
   {
        .diameter = 20,
        .step = 750,
        .is_default = 0,
        .bolt_major = {19840, 19980},
        .bolt_minor = {18923, 19166},
        .internal_major = {19188, 19378},
        .internal_minor = {20000, 20248},
        .drill_size = 19250
   },
   {
        .diameter = 20,
        .step = 500,
        .is_default = 0,
        .bolt_major = {19870, 19980},
        .bolt_minor = {19257, 19439},
        .internal_major = {19459, 19599},
        .internal_minor = {20000, 20192},
        .drill_size = 19500
   },
   {
        .diameter = 22,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {21580, 21950},
        .bolt_minor = {17970, 18704},
        .internal_major = {18752, 19252},
        .internal_minor = {22000, 22677},
        .drill_size = 19000
   },
   {
        .diameter = 22,
        .step = 2500,
        .is_default = 1,
        .bolt_major = {21620, 21960},
        .bolt_minor = {18624, 19252},
        .internal_major = {19294, 19744},
        .internal_minor = {22000, 22585},
        .drill_size = 19500
   },
   {
        .diameter = 22,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {21680, 21960},
        .bolt_minor = {19271, 19797},
        .internal_major = {19835, 20210},
        .internal_minor = {22000, 22501},
        .drill_size = 20000
   },
   {
        .diameter = 22,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {21730, 21970},
        .bolt_minor = {19930, 20344},
        .internal_major = {20376, 20676},
        .internal_minor = {22000, 22406},
        .drill_size = 20500
   },
   {
        .diameter = 22,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {21790, 21970},
        .bolt_minor = {20590, 20891},
        .internal_major = {20917, 21153},
        .internal_minor = {22000, 22304},
        .drill_size = 21000
   },
   {
        .diameter = 22,
        .step = 750,
        .is_default = 0,
        .bolt_major = {21840, 21980},
        .bolt_minor = {20923, 21166},
        .internal_major = {21188, 21378},
        .internal_minor = {22000, 22248},
        .drill_size = 21250
   },
   {
        .diameter = 22,
        .step = 500,
        .is_default = 0,
        .bolt_major = {21870, 21980},
        .bolt_minor = {21257, 21439},
        .internal_major = {21459, 21599},
        .internal_minor = {22000, 22192},
        .drill_size = 21500
   },
   {
        .diameter = 24,
        .step = 3000,
        .is_default = 1,
        .bolt_major = {23580, 23950},
        .bolt_minor = {19955, 20704},
        .internal_major = {20752, 21252},
        .internal_minor = {24000, 24698},
        .drill_size = 21000
   },
   {
        .diameter = 24,
        .step = 2500,
        .is_default = 0,
        .bolt_major = {23620, 23960},
        .bolt_minor = {20604, 21252},
        .internal_major = {21294, 21744},
        .internal_minor = {24000, 24611},
        .drill_size = 21500
   },
   {
        .diameter = 24,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {23680, 23960},
        .bolt_minor = {21261, 21797},
        .internal_major = {21835, 22210},
        .internal_minor = {24000, 24513},
        .drill_size = 22000
   },
   {
        .diameter = 24,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {23730, 23970},
        .bolt_minor = {21920, 22344},
        .internal_major = {22376, 22676},
        .internal_minor = {24000, 24417},
        .drill_size = 22500
   },
   {
        .diameter = 24,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {23790, 23970},
        .bolt_minor = {22583, 22891},
        .internal_major = {22917, 23153},
        .internal_minor = {24000, 24314},
        .drill_size = 23000
   },
   {
        .diameter = 24,
        .step = 750,
        .is_default = 0,
        .bolt_major = {23840, 23980},
        .bolt_minor = {22917, 23166},
        .internal_major = {23188, 23378},
        .internal_minor = {24000, 24258},
        .drill_size = 23250
   },
   {
        .diameter = 25,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {24680, 24960},
        .bolt_minor = {22261, 22797},
        .internal_major = {22835, 23210},
        .internal_minor = {25000, 25513},
        .drill_size = 23000
   },
   {
        .diameter = 25,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {24730, 24970},
        .bolt_minor = {22920, 23344},
        .internal_major = {23376, 23676},
        .internal_minor = {25000, 25416},
        .drill_size = 23500
   },
   {
        .diameter = 25,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {24790, 24970},
        .bolt_minor = {23583, 23891},
        .internal_major = {23917, 24153},
        .internal_minor = {25000, 25314},
        .drill_size = 24000
   },
   {
        .diameter = 26,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {25730, 25970},
        .bolt_minor = {23920, 24344},
        .internal_major = {24376, 24676},
        .internal_minor = {26000, 26417},
        .drill_size = 24500
   },
   {
        .diameter = 27,
        .step = 3000,
        .is_default = 1,
        .bolt_major = {26580, 26950},
        .bolt_minor = {22955, 23704},
        .internal_major = {23752, 24252},
        .internal_minor = {27000, 27698},
        .drill_size = 24000
   },
   {
        .diameter = 27,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {26680, 26960},
        .bolt_minor = {24261, 24797},
        .internal_major = {24835, 25210},
        .internal_minor = {27000, 27513},
        .drill_size = 25000
   },
   {
        .diameter = 27,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {26730, 26970},
        .bolt_minor = {24920, 25344},
        .internal_major = {25376, 25676},
        .internal_minor = {27000, 27417},
        .drill_size = 25500
   },
   {
        .diameter = 27,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {26790, 26970},
        .bolt_minor = {25583, 25891},
        .internal_major = {25917, 26153},
        .internal_minor = {27000, 27314},
        .drill_size = 26000
   },
   {
        .diameter = 27,
        .step = 750,
        .is_default = 0,
        .bolt_major = {26840, 26980},
        .bolt_minor = {25917, 26166},
        .internal_major = {26188, 26378},
        .internal_minor = {27000, 27258},
        .drill_size = 26250
   },
   {
        .diameter = 28,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {27680, 27960},
        .bolt_minor = {25261, 25797},
        .internal_major = {25835, 26210},
        .internal_minor = {28000, 28513},
        .drill_size = 26000
   },
   {
        .diameter = 28,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {27730, 27970},
        .bolt_minor = {25920, 26344},
        .internal_major = {26376, 26676},
        .internal_minor = {28000, 28417},
        .drill_size = 26500
   },
   {
        .diameter = 28,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {27790, 27970},
        .bolt_minor = {26583, 26891},
        .internal_major = {26917, 27153},
        .internal_minor = {28000, 28314},
        .drill_size = 27000
   },
   {
        .diameter = 30,
        .step = 3500,
        .is_default = 1,
        .bolt_major = {29520, 29950},
        .bolt_minor = {25306, 26158},
        .internal_major = {26211, 26771},
        .internal_minor = {30000, 30785},
        .drill_size = 26500
   },
   {
        .diameter = 30,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {29580, 29950},
        .bolt_minor = {25955, 26704},
        .internal_major = {26752, 27252},
        .internal_minor = {30000, 30698},
        .drill_size = 27000
   },
   {
        .diameter = 30,
        .step = 2500,
        .is_default = 0,
        .bolt_major = {29620, 29960},
        .bolt_minor = {26604, 27252},
        .internal_major = {27294, 27744},
        .internal_minor = {30000, 30611},
        .drill_size = 27500
   },
   {
        .diameter = 30,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {29680, 29960},
        .bolt_minor = {27261, 27797},
        .internal_major = {27835, 28210},
        .internal_minor = {30000, 30513},
        .drill_size = 28000
   },
   {
        .diameter = 30,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {29730, 29970},
        .bolt_minor = {27920, 28344},
        .internal_major = {28376, 28676},
        .internal_minor = {30000, 30416},
        .drill_size = 28500
   },
   {
        .diameter = 30,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {29790, 29970},
        .bolt_minor = {28583, 28891},
        .internal_major = {28917, 29153},
        .internal_minor = {30000, 30314},
        .drill_size = 29000
   },
   {
        .diameter = 30,
        .step = 750,
        .is_default = 0,
        .bolt_major = {29840, 29980},
        .bolt_minor = {28917, 29166},
        .internal_major = {29188, 29378},
        .internal_minor = {30000, 30258},
        .drill_size = 29250
   },
   {
        .diameter = 32,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {31680, 31960},
        .bolt_minor = {29261, 29797},
        .internal_major = {29835, 30210},
        .internal_minor = {32000, 32417},
        .drill_size = 30000
   },
   {
        .diameter = 32,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {31730, 31970},
        .bolt_minor = {29920, 30344},
        .internal_major = {30376, 30676},
        .internal_minor = {32000, 32417},
        .drill_size = 30500
   },
   {
        .diameter = 33,
        .step = 3500,
        .is_default = 1,
        .bolt_major = {32540, 32970},
        .bolt_minor = {28327, 29179},
        .internal_major = {29211, 29771},
        .internal_minor = {33000, 33785},
        .drill_size = 29500
   },
   {
        .diameter = 33,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {32580, 32950},
        .bolt_minor = {28955, 29704},
        .internal_major = {29752, 30252},
        .internal_minor = {33000, 33698},
        .drill_size = 30000
   },
   {
        .diameter = 33,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {32680, 32960},
        .bolt_minor = {30261, 30797},
        .internal_major = {30835, 31210},
        .internal_minor = {33000, 33513},
        .drill_size = 31000
   },
   {
        .diameter = 33,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {32729, 32970},
        .bolt_minor = {30920, 31344},
        .internal_major = {31376, 31676},
        .internal_minor = {33000, 33417},
        .drill_size = 31500
   },
   {
        .diameter = 33,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {32790, 32970},
        .bolt_minor = {31583, 31891},
        .internal_major = {31917, 32153},
        .internal_minor = {33000, 33314},
        .drill_size = 32000
   },
   {
        .diameter = 33,
        .step = 750,
        .is_default = 0,
        .bolt_major = {32840, 32980},
        .bolt_minor = {31917, 32165},
        .internal_major = {32188, 32378},
        .internal_minor = {33000, 33258},
        .drill_size = 32250
   },
   {
        .diameter = 35,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {34730, 34970},
        .bolt_minor = {32920, 33344},
        .internal_major = {33376, 33676},
        .internal_minor = {35000, 35416},
        .drill_size = 33500
   },
   {
        .diameter = 36,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {35470, 35940},
        .bolt_minor = {30654, 31610},
        .internal_major = {31670, 32270},
        .internal_minor = {36000, 36877},
        .drill_size = 32000
   },
   {
        .diameter = 36,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {35580, 35950},
        .bolt_minor = {31955, 32704},
        .internal_major = {32752, 33252},
        .internal_minor = {36000, 36698},
        .drill_size = 33000
   },
   {
        .diameter = 36,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {35680, 35960},
        .bolt_minor = {33261, 33797},
        .internal_major = {33835, 34210},
        .internal_minor = {36000, 36513},
        .drill_size = 34000
   },
   {
        .diameter = 36,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {35730, 35970},
        .bolt_minor = {33920, 34344},
        .internal_major = {34376, 34676},
        .internal_minor = {36000, 36417},
        .drill_size = 34500
   },
   {
        .diameter = 36,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {35790, 35970},
        .bolt_minor = {34583, 34891},
        .internal_major = {34917, 35153},
        .internal_minor = {36000, 36314},
        .drill_size = 35000
   },
   {
        .diameter = 38,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {37730, 37970},
        .bolt_minor = {35920, 36344},
        .internal_major = {36376, 36676},
        .internal_minor = {38000, 38417},
        .drill_size = 36500
   },
   {
        .diameter = 39,
        .step = 4000,
        .is_default = 1,
        .bolt_major = {38470, 38940},
        .bolt_minor = {33654, 34610},
        .internal_major = {34670, 35270},
        .internal_minor = {39000, 39877},
        .drill_size = 35000
   },
   {
        .diameter = 39,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {38580, 38950},
        .bolt_minor = {34955, 35704},
        .internal_major = {35752, 36252},
        .internal_minor = {39000, 39698},
        .drill_size = 36000
   },
   {
        .diameter = 39,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {38680, 38960},
        .bolt_minor = {36261, 36797},
        .internal_major = {36835, 37210},
        .internal_minor = {39000, 39513},
        .drill_size = 37000
   },
   {
        .diameter = 39,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {38730, 38970},
        .bolt_minor = {36920, 37344},
        .internal_major = {37376, 37676},
        .internal_minor = {39000, 39417},
        .drill_size = 37500
   },
   {
        .diameter = 39,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {38790, 38970},
        .bolt_minor = {37583, 37891},
        .internal_major = {37917, 38153},
        .internal_minor = {39000, 39314},
        .drill_size = 38000
   },
   {
        .diameter = 40,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {39580, 39950},
        .bolt_minor = {35955, 36704},
        .internal_major = {36752, 37252},
        .internal_minor = {40000, 40698},
        .drill_size = 37000
   },
   {
        .diameter = 40,
        .step = 2500,
        .is_default = 0,
        .bolt_major = {39620, 39960},
        .bolt_minor = {36604, 37252},
        .internal_major = {37294, 37744},
        .internal_minor = {40000, 40611},
        .drill_size = 37500
   },
   {
        .diameter = 40,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {39680, 39960},
        .bolt_minor = {37261, 37797},
        .internal_major = {37835, 38210},
        .internal_minor = {40000, 40513},
        .drill_size = 38000
   },
   {
        .diameter = 40,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {39730, 39970},
        .bolt_minor = {37920, 38344},
        .internal_major = {38376, 38676},
        .internal_minor = {40000, 40416},
        .drill_size = 38500
   },
   {
        .diameter = 42,
        .step = 4500,
        .is_default = 1,
        .bolt_major = {41440, 41940},
        .bolt_minor = {36006, 37066},
        .internal_major = {37129, 37799},
        .internal_minor = {42000, 42965},
        .drill_size = 37500
   },
   {
        .diameter = 42,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {41470, 41940},
        .bolt_minor = {36654, 37610},
        .internal_major = {37670, 38270},
        .internal_minor = {42000, 42877},
        .drill_size = 38000
   },
   {
        .diameter = 42,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {41580, 41950},
        .bolt_minor = {37955, 38704},
        .internal_major = {38752, 39252},
        .internal_minor = {42000, 42698},
        .drill_size = 39000
   },
   {
        .diameter = 42,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {41680, 41960},
        .bolt_minor = {39261, 39797},
        .internal_major = {39835, 40210},
        .internal_minor = {42000, 42513},
        .drill_size = 40000
   },
   {
        .diameter = 42,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {41730, 41970},
        .bolt_minor = {39920, 40344},
        .internal_major = {40376, 40676},
        .internal_minor = {42000, 42417},
        .drill_size = 40500
   },
   {
        .diameter = 42,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {41790, 41970},
        .bolt_minor = {40583, 40891},
        .internal_major = {40917, 41153},
        .internal_minor = {42000, 42314},
        .drill_size = 41000
   },
   {
        .diameter = 45,
        .step = 4500,
        .is_default = 0,
        .bolt_major = {44440, 44940},
        .bolt_minor = {39006, 40066},
        .internal_major = {40129, 40799},
        .internal_minor = {45000, 45965},
        .drill_size = 40500
   },
   {
        .diameter = 45,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {44470, 44940},
        .bolt_minor = {39654, 40610},
        .internal_major = {40670, 41270},
        .internal_minor = {45000, 45877},
        .drill_size = 41000
   },
   {
        .diameter = 45,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {44580, 44950},
        .bolt_minor = {40955, 41704},
        .internal_major = {41752, 42252},
        .internal_minor = {45000, 45698},
        .drill_size = 42000
   },
   {
        .diameter = 45,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {44680, 44960},
        .bolt_minor = {42261, 42797},
        .internal_major = {42835, 43210},
        .internal_minor = {45000, 45513},
        .drill_size = 43000
   },
   {
        .diameter = 45,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {44730, 44970},
        .bolt_minor = {42920, 43344},
        .internal_major = {43376, 43676},
        .internal_minor = {45000, 45416},
        .drill_size = 43500
   },
   {
        .diameter = 45,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {44790, 44970},
        .bolt_minor = {43583, 43891},
        .internal_major = {43917, 44153},
        .internal_minor = {45000, 45314},
        .drill_size = 44000
   },
   {
        .diameter = 48,
        .step = 5000,
        .is_default = 1,
        .bolt_major = {47400, 47930},
        .bolt_minor = {41351, 42516},
        .internal_major = {42587, 43297},
        .internal_minor = {48000, 49057},
        .drill_size = 43000
   },
   {
        .diameter = 48,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {47470, 47940},
        .bolt_minor = {42642, 43610},
        .internal_major = {43670, 44270},
        .internal_minor = {48000, 48892},
        .drill_size = 44000
   },
   {
        .diameter = 48,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {47580, 47950},
        .bolt_minor = {43943, 44704},
        .internal_major = {44752, 45252},
        .internal_minor = {48000, 48713},
        .drill_size = 45000
   },
   {
        .diameter = 48,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {47680, 47960},
        .bolt_minor = {45251, 45797},
        .internal_major = {45835, 46210},
        .internal_minor = {48000, 48525},
        .drill_size = 46000
   },
   {
        .diameter = 48,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {47730, 47970},
        .bolt_minor = {45910, 46344},
        .internal_major = {46376, 46676},
        .internal_minor = {48000, 48429},
        .drill_size = 46500
   },
   {
        .diameter = 50,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {49470, 49940},
        .bolt_minor = {44642, 45610},
        .internal_major = {45670, 46270},
        .internal_minor = {50000, 50892},
        .drill_size = 46000
   },
   {
        .diameter = 50,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {49580, 49950},
        .bolt_minor = {45943, 46704},
        .internal_major = {46752, 47252},
        .internal_minor = {50000, 50713},
        .drill_size = 47000
   },
   {
        .diameter = 50,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {49680, 49960},
        .bolt_minor = {47251, 47797},
        .internal_major = {47835, 48210},
        .internal_minor = {50000, 50525},
        .drill_size = 48000
   },
   {
        .diameter = 50,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {49730, 49970},
        .bolt_minor = {47910, 48344},
        .internal_major = {48376, 48676},
        .internal_minor = {50000, 50428},
        .drill_size = 48500
   },
   {
        .diameter = 52,
        .step = 5000,
        .is_default = 1,
        .bolt_major = {51400, 51930},
        .bolt_minor = {45365, 46516},
        .internal_major = {46587, 47297},
        .internal_minor = {52000, 53037},
        .drill_size = 47000
   },
   {
        .diameter = 52,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {51470, 51940},
        .bolt_minor = {46642, 47610},
        .internal_major = {47670, 48270},
        .internal_minor = {52000, 52892},
        .drill_size = 48000
   },
   {
        .diameter = 52,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {51580, 51950},
        .bolt_minor = {47943, 48704},
        .internal_major = {48752, 49252},
        .internal_minor = {52000, 52713},
        .drill_size = 49000
   },
   {
        .diameter = 52,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {51680, 51960},
        .bolt_minor = {49251, 49797},
        .internal_major = {49835, 50210},
        .internal_minor = {52000, 52525},
        .drill_size = 50000
   },
   {
        .diameter = 52,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {51730, 51970},
        .bolt_minor = {49910, 50344},
        .internal_major = {50376, 50676},
        .internal_minor = {52000, 52429},
        .drill_size = 50500
   },
   {
        .diameter = 55,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {54470, 54940},
        .bolt_minor = {49642, 50610},
        .internal_major = {50670, 51270},
        .internal_minor = {55000, 55892},
        .drill_size = 51000
   },
   {
        .diameter = 55,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {54580, 54950},
        .bolt_minor = {50943, 51704},
        .internal_major = {51752, 52252},
        .internal_minor = {55000, 55713},
        .drill_size = 52000
   },
   {
        .diameter = 55,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {54680, 54960},
        .bolt_minor = {52251, 52797},
        .internal_major = {52835, 53210},
        .internal_minor = {55000, 55525},
        .drill_size = 53000
   },
   {
        .diameter = 55,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {54730, 54970},
        .bolt_minor = {52910, 53344},
        .internal_major = {53376, 53676},
        .internal_minor = {55000, 55428},
        .drill_size = 53500
   },
   {
        .diameter = 56,
        .step = 5500,
        .is_default = 1,
        .bolt_major = {55370, 55930},
        .bolt_minor = {48700, 49971},
        .internal_major = {50046, 50796},
        .internal_minor = {56000, 57149},
        .drill_size = 50500
   },
   {
        .diameter = 56,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {55470, 55940},
        .bolt_minor = {50642, 51610},
        .internal_major = {51670, 52270},
        .internal_minor = {56000, 56892},
        .drill_size = 52000
   },
   {
        .diameter = 56,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {55580, 55950},
        .bolt_minor = {51943, 52704},
        .internal_major = {52752, 53252},
        .internal_minor = {56000, 56713},
        .drill_size = 53000
   },
   {
        .diameter = 56,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {55680, 55960},
        .bolt_minor = {53251, 53797},
        .internal_major = {53835, 54210},
        .internal_minor = {56000, 56525},
        .drill_size = 54000
   },
   {
        .diameter = 56,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {55730, 55970},
        .bolt_minor = {53910, 54344},
        .internal_major = {54376, 54676},
        .internal_minor = {56000, 56429},
        .drill_size = 54500
   },
   {
        .diameter = 56,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {55790, 55970},
        .bolt_minor = {54568, 54891},
        .internal_major = {54917, 55153},
        .internal_minor = {56000, 56334},
        .drill_size = 55000
   },
   {
        .diameter = 58,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {57470, 57940},
        .bolt_minor = {52642, 53610},
        .internal_major = {53670, 54270},
        .internal_minor = {58000, 58892},
        .drill_size = 54000
   },
   {
        .diameter = 58,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {57580, 57950},
        .bolt_minor = {53943, 54704},
        .internal_major = {54752, 55252},
        .internal_minor = {58000, 58713},
        .drill_size = 55000
   },
   {
        .diameter = 58,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {57680, 57960},
        .bolt_minor = {55251, 55797},
        .internal_major = {55835, 56210},
        .internal_minor = {58000, 58525},
        .drill_size = 56000
   },
   {
        .diameter = 58,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {57730, 57970},
        .bolt_minor = {55910, 56344},
        .internal_major = {56376, 56676},
        .internal_minor = {58000, 58429},
        .drill_size = 56500
   },
   {
        .diameter = 60,
        .step = 5500,
        .is_default = 1,
        .bolt_major = {59370, 59930},
        .bolt_minor = {52700, 53971},
        .internal_major = {54046, 54796},
        .internal_minor = {60000, 61149},
        .drill_size = 54500
   },
   {
        .diameter = 60,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {59470, 59940},
        .bolt_minor = {54642, 55610},
        .internal_major = {55670, 56270},
        .internal_minor = {60000, 60892},
        .drill_size = 56000
   },
   {
        .diameter = 60,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {59580, 59950},
        .bolt_minor = {55943, 56704},
        .internal_major = {56752, 57252},
        .internal_minor = {60000, 60713},
        .drill_size = 57000
   },
   {
        .diameter = 60,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {59680, 59960},
        .bolt_minor = {57251, 57797},
        .internal_major = {57835, 58210},
        .internal_minor = {60000, 60525},
        .drill_size = 58000
   },
   {
        .diameter = 60,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {59730, 59970},
        .bolt_minor = {57910, 58344},
        .internal_major = {58376, 58676},
        .internal_minor = {60000, 60428},
        .drill_size = 58500
   },
   {
        .diameter = 60,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {59790, 59970},
        .bolt_minor = {58568, 58891},
        .internal_major = {58917, 59153},
        .internal_minor = {60000, 60334},
        .drill_size = 59000
   },
   {
        .diameter = 62,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {61470, 61940},
        .bolt_minor = {56642, 57610},
        .internal_major = {57670, 58270},
        .internal_minor = {62000, 62892},
        .drill_size = 58000
   },
   {
        .diameter = 62,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {61580, 61950},
        .bolt_minor = {57943, 58704},
        .internal_major = {58752, 59252},
        .internal_minor = {62000, 62713},
        .drill_size = 59000
   },
   {
        .diameter = 62,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {61680, 61960},
        .bolt_minor = {59251, 59797},
        .internal_major = {59835, 60210},
        .internal_minor = {62000, 62525},
        .drill_size = 60000
   },
   {
        .diameter = 62,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {61730, 61970},
        .bolt_minor = {59910, 60344},
        .internal_major = {60376, 60676},
        .internal_minor = {62000, 62429},
        .drill_size = 60500
   },
   {
        .diameter = 63,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {62730, 62970},
        .bolt_minor = {60910, 61344},
        .internal_major = {61376, 61676},
        .internal_minor = {63000, 63429},
        .drill_size = 61500
   },
   {
        .diameter = 64,
        .step = 6000,
        .is_default = 1,
        .bolt_major = {63320, 63920},
        .bolt_minor = {56047, 57425},
        .internal_major = {57505, 58305},
        .internal_minor = {64000, 65241},
        .drill_size = 58000
   },
   {
        .diameter = 64,
        .step = 5500,
        .is_default = 0,
        .bolt_major = {63370, 63930},
        .bolt_minor = {56700, 57971},
        .internal_major = {58046, 58796},
        .internal_minor = {64000, 65149},
        .drill_size = 58500
   },
   {
        .diameter = 64,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {63470, 63940},
        .bolt_minor = {58642, 59610},
        .internal_major = {59670, 60270},
        .internal_minor = {64000, 64891},
        .drill_size = 60000
   },
   {
        .diameter = 64,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {63580, 63950},
        .bolt_minor = {59943, 60704},
        .internal_major = {60752, 61252},
        .internal_minor = {64000, 64712},
        .drill_size = 61000
   },
   {
        .diameter = 64,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {63680, 63960},
        .bolt_minor = {61251, 61797},
        .internal_major = {61835, 62210},
        .internal_minor = {64000, 64525},
        .drill_size = 62000
   },
   {
        .diameter = 64,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {63730, 63970},
        .bolt_minor = {61910, 62344},
        .internal_major = {62376, 62676},
        .internal_minor = {64000, 64429},
        .drill_size = 62500
   },
   {
        .diameter = 64,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {63790, 63970},
        .bolt_minor = {62568, 62891},
        .internal_major = {62917, 63153},
        .internal_minor = {64000, 64334},
        .drill_size = 63000
   },
   {
        .diameter = 65,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {64470, 64940},
        .bolt_minor = {59642, 60610},
        .internal_major = {60670, 61270},
        .internal_minor = {65000, 65892},
        .drill_size = 61000
   },
   {
        .diameter = 65,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {64580, 64950},
        .bolt_minor = {60943, 61704},
        .internal_major = {61752, 62252},
        .internal_minor = {65000, 65713},
        .drill_size = 62000
   },
   {
        .diameter = 65,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {64680, 64959},
        .bolt_minor = {62251, 62797},
        .internal_major = {62835, 63210},
        .internal_minor = {65000, 65525},
        .drill_size = 63000
   },
   {
        .diameter = 65,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {64730, 64970},
        .bolt_minor = {62910, 63344},
        .internal_major = {63376, 63676},
        .internal_minor = {65000, 65428},
        .drill_size = 63500
   },
   {
        .diameter = 68,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {67320, 67920},
        .bolt_minor = {60047, 61425},
        .internal_major = {61505, 62305},
        .internal_minor = {68000, 69241},
        .drill_size = 62000
   },
   {
        .diameter = 68,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {67470, 67940},
        .bolt_minor = {62642, 63610},
        .internal_major = {63670, 64269},
        .internal_minor = {68000, 68892},
        .drill_size = 64000
   },
   {
        .diameter = 68,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {67580, 67950},
        .bolt_minor = {63943, 64703},
        .internal_major = {64751, 65251},
        .internal_minor = {68000, 68713},
        .drill_size = 65000
   },
   {
        .diameter = 68,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {67680, 67960},
        .bolt_minor = {65251, 65797},
        .internal_major = {65835, 66210},
        .internal_minor = {68000, 68525},
        .drill_size = 66000
   },
   {
        .diameter = 68,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {67730, 67970},
        .bolt_minor = {65910, 66344},
        .internal_major = {66376, 66676},
        .internal_minor = {68000, 68429},
        .drill_size = 66500
   },
   {
        .diameter = 68,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {67790, 67970},
        .bolt_minor = {66568, 66891},
        .internal_major = {66917, 67153},
        .internal_minor = {68000, 68334},
        .drill_size = 67000
   },
   {
        .diameter = 70,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {69320, 69920},
        .bolt_minor = {62047, 63425},
        .internal_major = {63505, 64305},
        .internal_minor = {70000, 71241},
        .drill_size = 64000
   },
   {
        .diameter = 70,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {69470, 69940},
        .bolt_minor = {64641, 65610},
        .internal_major = {65670, 66270},
        .internal_minor = {70000, 70892},
        .drill_size = 66000
   },
   {
        .diameter = 70,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {69580, 69950},
        .bolt_minor = {65943, 66704},
        .internal_major = {66752, 67252},
        .internal_minor = {70000, 70713},
        .drill_size = 67000
   },
   {
        .diameter = 70,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {69680, 69960},
        .bolt_minor = {67251, 67797},
        .internal_major = {67835, 68210},
        .internal_minor = {70000, 70525},
        .drill_size = 68000
   },
   {
        .diameter = 70,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {69730, 69970},
        .bolt_minor = {67910, 68344},
        .internal_major = {68376, 68676},
        .internal_minor = {70000, 70428},
        .drill_size = 68500
   },
   {
        .diameter = 72,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {71320, 71920},
        .bolt_minor = {64047, 65425},
        .internal_major = {65504, 66305},
        .internal_minor = {72000, 73241},
        .drill_size = 66000
   },
   {
        .diameter = 72,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {71470, 71940},
        .bolt_minor = {66642, 67610},
        .internal_major = {67670, 68270},
        .internal_minor = {72000, 72892},
        .drill_size = 68000
   },
   {
        .diameter = 72,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {71580, 71950},
        .bolt_minor = {67943, 68704},
        .internal_major = {68752, 69252},
        .internal_minor = {72000, 72713},
        .drill_size = 69000
   },
   {
        .diameter = 72,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {71680, 71960},
        .bolt_minor = {69251, 69797},
        .internal_major = {69835, 70210},
        .internal_minor = {72000, 72525},
        .drill_size = 70000
   },
   {
        .diameter = 72,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {71730, 71970},
        .bolt_minor = {69910, 70344},
        .internal_major = {70376, 70676},
        .internal_minor = {72000, 72429},
        .drill_size = 70500
   },
   {
        .diameter = 72,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {71790, 71970},
        .bolt_minor = {70568, 70891},
        .internal_major = {70917, 71153},
        .internal_minor = {72000, 72334},
        .drill_size = 71000
   },
   {
        .diameter = 75,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {74320, 74920},
        .bolt_minor = {67047, 68425},
        .internal_major = {68505, 69305},
        .internal_minor = {75000, 76241},
        .drill_size = 69000
   },
   {
        .diameter = 75,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {74470, 74940},
        .bolt_minor = {69642, 70610},
        .internal_major = {70670, 71270},
        .internal_minor = {75000, 75892},
        .drill_size = 71000
   },
   {
        .diameter = 75,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {74580, 74950},
        .bolt_minor = {70943, 71704},
        .internal_major = {71752, 72252},
        .internal_minor = {75000, 75713},
        .drill_size = 72000
   },
   {
        .diameter = 75,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {74680, 74960},
        .bolt_minor = {72251, 72797},
        .internal_major = {72835, 73210},
        .internal_minor = {75000, 75525},
        .drill_size = 73000
   },
   {
        .diameter = 75,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {74730, 74970},
        .bolt_minor = {72910, 73344},
        .internal_major = {73376, 73670},
        .internal_minor = {75000, 75428},
        .drill_size = 73500
   },
   {
        .diameter = 76,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {75320, 75920},
        .bolt_minor = {68047, 69425},
        .internal_major = {69505, 70305},
        .internal_minor = {76000, 77241},
        .drill_size = 70000
   },
   {
        .diameter = 76,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {75470, 75940},
        .bolt_minor = {70642, 71610},
        .internal_major = {71670, 72270},
        .internal_minor = {76000, 76892},
        .drill_size = 72000
   },
   {
        .diameter = 76,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {75580, 75950},
        .bolt_minor = {71943, 72704},
        .internal_major = {72752, 73250},
        .internal_minor = {76000, 76713},
        .drill_size = 73000
   },
   {
        .diameter = 76,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {75680, 75960},
        .bolt_minor = {73251, 73797},
        .internal_major = {73835, 74210},
        .internal_minor = {76000, 76525},
        .drill_size = 74000
   },
   {
        .diameter = 76,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {75730, 75970},
        .bolt_minor = {73910, 74344},
        .internal_major = {74376, 74676},
        .internal_minor = {76000, 76429},
        .drill_size = 74500
   },
   {
        .diameter = 76,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {75790, 75970},
        .bolt_minor = {74568, 74891},
        .internal_major = {74917, 75153},
        .internal_minor = {76000, 76334},
        .drill_size = 75000
   },
   {
        .diameter = 78,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {77680, 77960},
        .bolt_minor = {75251, 75797},
        .internal_major = {75835, 76210},
        .internal_minor = {78000, 78525},
        .drill_size = 76000
   },
   {
        .diameter = 80,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {79320, 79920},
        .bolt_minor = {72047, 73425},
        .internal_major = {73505, 74305},
        .internal_minor = {80000, 81241},
        .drill_size = 74000
   },
   {
        .diameter = 80,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {79340, 79940},
        .bolt_minor = {74642, 75610},
        .internal_major = {75670, 76270},
        .internal_minor = {80000, 80892},
        .drill_size = 76000
   },
   {
        .diameter = 80,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {79580, 79950},
        .bolt_minor = {75943, 76704},
        .internal_major = {76752, 77252},
        .internal_minor = {80000, 80713},
        .drill_size = 77000
   },
   {
        .diameter = 80,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {79680, 79960},
        .bolt_minor = {77251, 77797},
        .internal_major = {77835, 78210},
        .internal_minor = {80000, 80525},
        .drill_size = 78000
   },
   {
        .diameter = 80,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {79730, 79970},
        .bolt_minor = {77910, 78344},
        .internal_major = {78376, 78676},
        .internal_minor = {80000, 80428},
        .drill_size = 78500
   },
   {
        .diameter = 80,
        .step = 1000,
        .is_default = 0,
        .bolt_major = {79790, 79970},
        .bolt_minor = {78568, 78891},
        .internal_major = {78917, 79153},
        .internal_minor = {80000, 80334},
        .drill_size = 79000
   },
   {
        .diameter = 82,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {81680, 81960},
        .bolt_minor = {79251, 79797},
        .internal_major = {79835, 80210},
        .internal_minor = {82000, 82525},
        .drill_size = 80000
   },
   {
        .diameter = 85,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {84320, 84920},
        .bolt_minor = {77047, 78425},
        .internal_major = {78505, 79305},
        .internal_minor = {85000, 86241},
        .drill_size = 79000
   },
   {
        .diameter = 85,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {84470, 84940},
        .bolt_minor = {79642, 80610},
        .internal_major = {80670, 81270},
        .internal_minor = {85000, 85892},
        .drill_size = 81000
   },
   {
        .diameter = 85,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {84580, 84950},
        .bolt_minor = {80943, 81704},
        .internal_major = {81752, 82252},
        .internal_minor = {85000, 85713},
        .drill_size = 82000
   },
   {
        .diameter = 85,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {84680, 84960},
        .bolt_minor = {82251, 82797},
        .internal_major = {82835, 83210},
        .internal_minor = {85000, 85525},
        .drill_size = 83000
   },
   {
        .diameter = 85,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {84730, 84970},
        .bolt_minor = {82910, 83344},
        .internal_major = {83376, 83676},
        .internal_minor = {85000, 85429},
        .drill_size = 83500
   },
   {
        .diameter = 90,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {89320, 89920},
        .bolt_minor = {82047, 83425},
        .internal_major = {83505, 84305},
        .internal_minor = {90000, 91241},
        .drill_size = 84000
   },
   {
        .diameter = 90,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {89470, 89940},
        .bolt_minor = {84642, 85610},
        .internal_major = {85670, 86270},
        .internal_minor = {90000, 90892},
        .drill_size = 86000
   },
   {
        .diameter = 90,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {89580, 89950},
        .bolt_minor = {85943, 86704},
        .internal_major = {86752, 87252},
        .internal_minor = {90000, 90713},
        .drill_size = 87000
   },
   {
        .diameter = 90,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {89680, 89960},
        .bolt_minor = {87251, 87797},
        .internal_major = {87835, 88210},
        .internal_minor = {90000, 90525},
        .drill_size = 88000
   },
   {
        .diameter = 90,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {89730, 89970},
        .bolt_minor = {87910, 88344},
        .internal_major = {88376, 88676},
        .internal_minor = {90000, 90429},
        .drill_size = 88500
   },
   {
        .diameter = 95,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {94320, 94920},
        .bolt_minor = {87027, 88425},
        .internal_major = {88505, 89305},
        .internal_minor = {95000, 96266},
        .drill_size = 89000
   },
   {
        .diameter = 95,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {94470, 94940},
        .bolt_minor = {89628, 90610},
        .internal_major = {90670, 91270},
        .internal_minor = {95000, 95912},
        .drill_size = 91000
   },
   {
        .diameter = 95,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {94580, 94950},
        .bolt_minor = {90931, 91704},
        .internal_major = {91752, 92252},
        .internal_minor = {95000, 95733},
        .drill_size = 92000
   },
   {
        .diameter = 95,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {94680, 94960},
        .bolt_minor = {92241, 92797},
        .internal_major = {92835, 93210},
        .internal_minor = {95000, 95539},
        .drill_size = 93000
   },
   {
        .diameter = 95,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {94730, 94970},
        .bolt_minor = {92890, 93344},
        .internal_major = {93376, 93676},
        .internal_minor = {95000, 95453},
        .drill_size = 93500
   },
   {
        .diameter = 100,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {99320, 99920},
        .bolt_minor = {92027, 93425},
        .internal_major = {93505, 94305},
        .internal_minor = {100000, 101270},
        .drill_size = 94000
   },
   {
        .diameter = 100,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {99470, 99940},
        .bolt_minor = {94628, 95610},
        .internal_major = {95670, 96270},
        .internal_minor = {100000, 100910},
        .drill_size = 96000
   },
   {
        .diameter = 100,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {99580, 99950},
        .bolt_minor = {95931, 96704},
        .internal_major = {96752, 97252},
        .internal_minor = {100000, 100730},
        .drill_size = 97000
   },
   {
        .diameter = 100,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {99680, 99960},
        .bolt_minor = {97241, 97797},
        .internal_major = {97835, 98210},
        .internal_minor = {100000, 100540},
        .drill_size = 98000
   },
   {
        .diameter = 100,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {99730, 99970},
        .bolt_minor = {97890, 98344},
        .internal_major = {98376, 98676},
        .internal_minor = {100000, 100450},
        .drill_size = 98500
   },
   {
        .diameter = 105,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {104300, 104900},
        .bolt_minor = {97027, 98425},
        .internal_major = {98505, 99305},
        .internal_minor = {105000, 106270},
        .drill_size = 99000
   },
   {
        .diameter = 105,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {104500, 104900},
        .bolt_minor = {99628, 100610},
        .internal_major = {100670, 101270},
        .internal_minor = {105000, 105910},
        .drill_size = 101000
   },
   {
        .diameter = 105,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {104600, 105000},
        .bolt_minor = {100930, 101700},
        .internal_major = {101750, 102250},
        .internal_minor = {105000, 105730},
        .drill_size = 102000
   },
   {
        .diameter = 105,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {104700, 105000},
        .bolt_minor = {102240, 102800},
        .internal_major = {102840, 103210},
        .internal_minor = {105000, 105540},
        .drill_size = 103000
   },
   {
        .diameter = 105,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {104700, 105000},
        .bolt_minor = {102890, 103340},
        .internal_major = {103380, 103680},
        .internal_minor = {105000, 105450},
        .drill_size = 103500
   },
   {
        .diameter = 110,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {109300, 109900},
        .bolt_minor = {102030, 103430},
        .internal_major = {103510, 104310},
        .internal_minor = {110000, 111270},
        .drill_size = 104000
   },
   {
        .diameter = 110,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {109500, 109900},
        .bolt_minor = {104630, 105610},
        .internal_major = {105670, 106270},
        .internal_minor = {110000, 110910},
        .drill_size = 106000
   },
   {
        .diameter = 110,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {109600, 110000},
        .bolt_minor = {105930, 106700},
        .internal_major = {106750, 107250},
        .internal_minor = {110000, 110730},
        .drill_size = 107000
   },
   {
        .diameter = 110,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {109700, 110000},
        .bolt_minor = {107240, 107800},
        .internal_major = {107840, 108210},
        .internal_minor = {110000, 110540},
        .drill_size = 108000
   },
   {
        .diameter = 110,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {109700, 110000},
        .bolt_minor = {107890, 108340},
        .internal_major = {108380, 108680},
        .internal_minor = {110000, 110450},
        .drill_size = 108500
   },
   {
        .diameter = 115,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {114300, 114900},
        .bolt_minor = {107030, 108430},
        .internal_major = {108510, 109310},
        .internal_minor = {115000, 116270},
        .drill_size = 109000
   },
   {
        .diameter = 115,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {114500, 114900},
        .bolt_minor = {109630, 110610},
        .internal_major = {110670, 111270},
        .internal_minor = {115000, 115910},
        .drill_size = 111000
   },
   {
        .diameter = 115,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {114600, 115000},
        .bolt_minor = {110930, 111700},
        .internal_major = {111750, 112250},
        .internal_minor = {115000, 115730},
        .drill_size = 112000
   },
   {
        .diameter = 115,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {114700, 115000},
        .bolt_minor = {112240, 112800},
        .internal_major = {112840, 113210},
        .internal_minor = {115000, 115540},
        .drill_size = 113000
   },
   {
        .diameter = 115,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {114700, 115000},
        .bolt_minor = {112890, 113340},
        .internal_major = {113380, 113680},
        .internal_minor = {115000, 115450},
        .drill_size = 113500
   },
   {
        .diameter = 120,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {119300, 119900},
        .bolt_minor = {112030, 113430},
        .internal_major = {113510, 114310},
        .internal_minor = {120000, 121270},
        .drill_size = 114000
   },
   {
        .diameter = 120,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {119500, 119900},
        .bolt_minor = {114630, 115610},
        .internal_major = {115670, 116270},
        .internal_minor = {120000, 120910},
        .drill_size = 116000
   },
   {
        .diameter = 120,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {119600, 120000},
        .bolt_minor = {115930, 116700},
        .internal_major = {116750, 117250},
        .internal_minor = {120000, 120730},
        .drill_size = 117000
   },
   {
        .diameter = 120,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {119700, 120000},
        .bolt_minor = {117240, 117800},
        .internal_major = {117840, 118210},
        .internal_minor = {120000, 120540},
        .drill_size = 118000
   },
   {
        .diameter = 120,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {119700, 120000},
        .bolt_minor = {117890, 118340},
        .internal_major = {118380, 118680},
        .internal_minor = {120000, 120450},
        .drill_size = 118500
   },
   {
        .diameter = 125,
        .step = 8000,
        .is_default = 0,
        .bolt_major = {124200, 124900},
        .bolt_minor = {114440, 116240},
        .internal_major = {116340, 117340},
        .internal_minor = {125000, 126610},
        .drill_size = 117000
   },
   {
        .diameter = 125,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {124200, 124900},
        .bolt_minor = {117030, 118430},
        .internal_major = {118510, 119310},
        .internal_minor = {125000, 126270},
        .drill_size = 119000
   },
   {
        .diameter = 125,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {124300, 124900},
        .bolt_minor = {119630, 120610},
        .internal_major = {120670, 121270},
        .internal_minor = {125000, 125910},
        .drill_size = 121000
   },
   {
        .diameter = 125,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {124500, 125000},
        .bolt_minor = {120930, 121700},
        .internal_major = {121750, 122250},
        .internal_minor = {125000, 125730},
        .drill_size = 122000
   },
   {
        .diameter = 125,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {124600, 125000},
        .bolt_minor = {122240, 122800},
        .internal_major = {122840, 123210},
        .internal_minor = {125000, 125540},
        .drill_size = 123000
   },
   {
        .diameter = 125,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {124700, 125000},
        .bolt_minor = {122890, 123340},
        .internal_major = {123380, 123680},
        .internal_minor = {125000, 125450},
        .drill_size = 123500
   },
   {
        .diameter = 130,
        .step = 8000,
        .is_default = 0,
        .bolt_major = {124700, 129900},
        .bolt_minor = {119440, 121240},
        .internal_major = {121340, 122340},
        .internal_minor = {130000, 131610},
        .drill_size = 122000
   },
   {
        .diameter = 130,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {129199, 129900},
        .bolt_minor = {122030, 123430},
        .internal_major = {123510, 124310},
        .internal_minor = {130000, 131270},
        .drill_size = 124000
   },
   {
        .diameter = 130,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {129300, 129900},
        .bolt_minor = {124630, 125610},
        .internal_major = {125670, 126270},
        .internal_minor = {130000, 130910},
        .drill_size = 126000
   },
   {
        .diameter = 130,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {129500, 130000},
        .bolt_minor = {125930, 126700},
        .internal_major = {126750, 127250},
        .internal_minor = {130000, 130729},
        .drill_size = 127000
   },
   {
        .diameter = 130,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {129600, 130000},
        .bolt_minor = {127240, 127800},
        .internal_major = {127840, 128210},
        .internal_minor = {130000, 130539},
        .drill_size = 128000
   },
   {
        .diameter = 130,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {129699, 130000},
        .bolt_minor = {127890, 128340},
        .internal_major = {128380, 128680},
        .internal_minor = {130000, 130449},
        .drill_size = 128500
   },
   {
        .diameter = 135,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {129699, 134900},
        .bolt_minor = {127030, 128430},
        .internal_major = {128509, 129310},
        .internal_minor = {135000, 136270},
        .drill_size = 129000
   },
   {
        .diameter = 135,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {134300, 134900},
        .bolt_minor = {129630, 130610},
        .internal_major = {130669, 131270},
        .internal_minor = {135000, 135910},
        .drill_size = 131000
   },
   {
        .diameter = 135,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {134500, 135000},
        .bolt_minor = {130930, 131700},
        .internal_major = {131750, 132250},
        .internal_minor = {135000, 135730},
        .drill_size = 132000
   },
   {
        .diameter = 135,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {134600, 135000},
        .bolt_minor = {132240, 132800},
        .internal_major = {132840, 133210},
        .internal_minor = {135000, 135540},
        .drill_size = 133000
   },
   {
        .diameter = 135,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {134700, 135000},
        .bolt_minor = {132890, 133340},
        .internal_major = {133380, 133680},
        .internal_minor = {135000, 135450},
        .drill_size = 133500
   },
   {
        .diameter = 140,
        .step = 8000,
        .is_default = 0,
        .bolt_major = {134700, 139900},
        .bolt_minor = {129440, 131240},
        .internal_major = {131340, 132340},
        .internal_minor = {140000, 141610},
        .drill_size = 132000
   },
   {
        .diameter = 140,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {139200, 139900},
        .bolt_minor = {132030, 133430},
        .internal_major = {133510, 134310},
        .internal_minor = {140000, 141210},
        .drill_size = 134000
   },
   {
        .diameter = 140,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {139500, 139900},
        .bolt_minor = {134630, 135610},
        .internal_major = {135670, 136270},
        .internal_minor = {140000, 140910},
        .drill_size = 136000
   },
   {
        .diameter = 140,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {139600, 140000},
        .bolt_minor = {135930, 136700},
        .internal_major = {136750, 137250},
        .internal_minor = {140000, 140730},
        .drill_size = 137000
   },
   {
        .diameter = 140,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {139700, 140000},
        .bolt_minor = {137240, 137800},
        .internal_major = {137840, 138210},
        .internal_minor = {140000, 140540},
        .drill_size = 138000
   },
   {
        .diameter = 140,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {139700, 140000},
        .bolt_minor = {137890, 138340},
        .internal_major = {138380, 138680},
        .internal_minor = {140000, 140450},
        .drill_size = 138500
   },
   {
        .diameter = 145,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {144300, 144900},
        .bolt_minor = {137030, 138430},
        .internal_major = {138510, 139310},
        .internal_minor = {145000, 146270},
        .drill_size = 139000
   },
   {
        .diameter = 145,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {144500, 144900},
        .bolt_minor = {139630, 140610},
        .internal_major = {140670, 141270},
        .internal_minor = {145000, 145910},
        .drill_size = 141000
   },
   {
        .diameter = 145,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {144600, 145000},
        .bolt_minor = {140930, 141700},
        .internal_major = {141750, 142250},
        .internal_minor = {145000, 145730},
        .drill_size = 142000
   },
   {
        .diameter = 145,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {144700, 145000},
        .bolt_minor = {142240, 142800},
        .internal_major = {142840, 143210},
        .internal_minor = {145000, 145540},
        .drill_size = 143000
   },
   {
        .diameter = 145,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {144700, 145000},
        .bolt_minor = {142890, 143340},
        .internal_major = {143380, 143680},
        .internal_minor = {145000, 145450},
        .drill_size = 143500
   },
   {
        .diameter = 150,
        .step = 8000,
        .is_default = 0,
        .bolt_major = {149200, 149900},
        .bolt_minor = {139440, 141240},
        .internal_major = {141340, 142340},
        .internal_minor = {150000, 151610},
        .drill_size = 142000
   },
   {
        .diameter = 150,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {149300, 149900},
        .bolt_minor = {142030, 143430},
        .internal_major = {143510, 144310},
        .internal_minor = {150000, 151270},
        .drill_size = 144000
   },
   {
        .diameter = 150,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {149500, 149900},
        .bolt_minor = {144630, 145610},
        .internal_major = {145670, 146270},
        .internal_minor = {150000, 150910},
        .drill_size = 146000
   },
   {
        .diameter = 150,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {149600, 150000},
        .bolt_minor = {145930, 146700},
        .internal_major = {146750, 147250},
        .internal_minor = {150000, 150730},
        .drill_size = 147000
   },
   {
        .diameter = 150,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {149700, 150000},
        .bolt_minor = {147240, 147800},
        .internal_major = {147840, 148210},
        .internal_minor = {150000, 150540},
        .drill_size = 148000
   },
   {
        .diameter = 150,
        .step = 1500,
        .is_default = 0,
        .bolt_major = {149700, 150000},
        .bolt_minor = {147890, 148340},
        .internal_major = {148380, 148680},
        .internal_minor = {150000, 150450},
        .drill_size = 148500
   },
   {
        .diameter = 155,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {154300, 154900},
        .bolt_minor = {147030, 148340},
        .internal_major = {148510, 149310},
        .internal_minor = {155000, 156270},
        .drill_size = 149000
   },
   {
        .diameter = 155,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {154500, 154900},
        .bolt_minor = {149630, 150610},
        .internal_major = {150670, 151270},
        .internal_minor = {155000, 155910},
        .drill_size = 151000
   },
   {
        .diameter = 155,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {154600, 155000},
        .bolt_minor = {150930, 151700},
        .internal_major = {151750, 152250},
        .internal_minor = {155000, 155730},
        .drill_size = 152000
   },
   {
        .diameter = 155,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {154700, 155000},
        .bolt_minor = {152240, 152800},
        .internal_major = {152840, 153210},
        .internal_minor = {155000, 155540},
        .drill_size = 153000
   },
   {
        .diameter = 160,
        .step = 8000,
        .is_default = 0,
        .bolt_major = {159200, 159900},
        .bolt_minor = {149440, 151240},
        .internal_major = {151340, 152340},
        .internal_minor = {160000, 161610},
        .drill_size = 152000
   },
   {
        .diameter = 160,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {159300, 159900},
        .bolt_minor = {152030, 153430},
        .internal_major = {153510, 154310},
        .internal_minor = {160000, 161270},
        .drill_size = 154000
   },
   {
        .diameter = 160,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {159500, 159900},
        .bolt_minor = {154630, 155610},
        .internal_major = {155670, 156270},
        .internal_minor = {160000, 160910},
        .drill_size = 156000
   },
   {
        .diameter = 160,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {159600, 160000},
        .bolt_minor = {155930, 156700},
        .internal_major = {156750, 157250},
        .internal_minor = {160000, 160730},
        .drill_size = 157000
   },
   {
        .diameter = 160,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {159700, 160000},
        .bolt_minor = {157240, 157800},
        .internal_major = {157840, 158210},
        .internal_minor = {160000, 160540},
        .drill_size = 158000
   },
   {
        .diameter = 165,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {164300, 164900},
        .bolt_minor = {157030, 158430},
        .internal_major = {158510, 159310},
        .internal_minor = {165000, 166270},
        .drill_size = 159000
   },
   {
        .diameter = 165,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {164500, 164900},
        .bolt_minor = {159630, 160610},
        .internal_major = {160670, 161270},
        .internal_minor = {165000, 165910},
        .drill_size = 161000
   },
   {
        .diameter = 165,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {164600, 165000},
        .bolt_minor = {160930, 161700},
        .internal_major = {161750, 162250},
        .internal_minor = {165000, 165730},
        .drill_size = 162000
   },
   {
        .diameter = 165,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {164700, 165000},
        .bolt_minor = {162240, 162800},
        .internal_major = {162840, 163210},
        .internal_minor = {165000, 165540},
        .drill_size = 163000
   },
   {
        .diameter = 170,
        .step = 8000,
        .is_default = 0,
        .bolt_major = {169200, 169900},
        .bolt_minor = {159440, 161240},
        .internal_major = {161340, 162340},
        .internal_minor = {170000, 171610},
        .drill_size = 162000
   },
   {
        .diameter = 170,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {169300, 169900},
        .bolt_minor = {162030, 163430},
        .internal_major = {163510, 164310},
        .internal_minor = {170000, 171270},
        .drill_size = 164000
   },
   {
        .diameter = 170,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {169500, 169900},
        .bolt_minor = {164630, 165610},
        .internal_major = {165670, 166270},
        .internal_minor = {170000, 170910},
        .drill_size = 166000
   },
   {
        .diameter = 170,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {169600, 170000},
        .bolt_minor = {165930, 166700},
        .internal_major = {166750, 167250},
        .internal_minor = {170000, 170730},
        .drill_size = 167000
   },
   {
        .diameter = 170,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {169700, 170000},
        .bolt_minor = {167240, 167800},
        .internal_major = {167840, 168210},
        .internal_minor = {170000, 170540},
        .drill_size = 168000
   },
   {
        .diameter = 175,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {174300, 174900},
        .bolt_minor = {167030, 168430},
        .internal_major = {168510, 169310},
        .internal_minor = {175000, 176270},
        .drill_size = 169000
   },
   {
        .diameter = 175,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {174500, 174900},
        .bolt_minor = {169630, 170610},
        .internal_major = {170670, 171270},
        .internal_minor = {175000, 175910},
        .drill_size = 171000
   },
   {
        .diameter = 175,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {174600, 175000},
        .bolt_minor = {170930, 171700},
        .internal_major = {171750, 172250},
        .internal_minor = {175000, 175730},
        .drill_size = 172000
   },
   {
        .diameter = 175,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {174700, 175000},
        .bolt_minor = {172240, 172800},
        .internal_major = {172840, 173210},
        .internal_minor = {175000, 175540},
        .drill_size = 173000
   },
   {
        .diameter = 180,
        .step = 8000,
        .is_default = 0,
        .bolt_major = {179200, 179900},
        .bolt_minor = {169440, 171240},
        .internal_major = {171340, 172340},
        .internal_minor = {180000, 181610},
        .drill_size = 172000
   },
   {
        .diameter = 180,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {179300, 179900},
        .bolt_minor = {172030, 173430},
        .internal_major = {173510, 174310},
        .internal_minor = {180000, 181270},
        .drill_size = 174000
   },
   {
        .diameter = 180,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {179500, 179900},
        .bolt_minor = {174630, 175610},
        .internal_major = {175670, 176270},
        .internal_minor = {180000, 180910},
        .drill_size = 176000
   },
   {
        .diameter = 180,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {179600, 180000},
        .bolt_minor = {175930, 176700},
        .internal_major = {176750, 177250},
        .internal_minor = {180000, 180730},
        .drill_size = 177000
   },
   {
        .diameter = 180,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {179700, 180000},
        .bolt_minor = {177240, 177800},
        .internal_major = {177840, 178210},
        .internal_minor = {180000, 180540},
        .drill_size = 178000
   },
   {
        .diameter = 185,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {184300, 184900},
        .bolt_minor = {177010, 178430},
        .internal_major = {178510, 179310},
        .internal_minor = {185000, 186290},
        .drill_size = 179000
   },
   {
        .diameter = 185,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {184500, 184900},
        .bolt_minor = {179600, 180610},
        .internal_major = {180670, 181270},
        .internal_minor = {185000, 185950},
        .drill_size = 181000
   },
   {
        .diameter = 185,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {184600, 185000},
        .bolt_minor = {180910, 181700},
        .internal_major = {181750, 182250},
        .internal_minor = {185000, 185770},
        .drill_size = 182000
   },
   {
        .diameter = 185,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {184700, 185000},
        .bolt_minor = {182220, 182800},
        .internal_major = {182840, 183210},
        .internal_minor = {185000, 185570},
        .drill_size = 183000
   },
   {
        .diameter = 190,
        .step = 8000,
        .is_default = 0,
        .bolt_major = {189200, 189900},
        .bolt_minor = {179420, 181240},
        .internal_major = {181340, 182340},
        .internal_minor = {190000, 191630},
        .drill_size = 182000
   },
   {
        .diameter = 190,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {189300, 189900},
        .bolt_minor = {182010, 183430},
        .internal_major = {183510, 184310},
        .internal_minor = {190000, 191290},
        .drill_size = 184000
   },
   {
        .diameter = 190,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {189500, 189900},
        .bolt_minor = {184600, 185610},
        .internal_major = {185670, 186270},
        .internal_minor = {190000, 190950},
        .drill_size = 186000
   },
   {
        .diameter = 190,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {189600, 190000},
        .bolt_minor = {185910, 186700},
        .internal_major = {186750, 187250},
        .internal_minor = {190000, 190770},
        .drill_size = 187000
   },
   {
        .diameter = 190,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {189700, 190000},
        .bolt_minor = {187220, 187800},
        .internal_major = {187840, 188210},
        .internal_minor = {190000, 190570},
        .drill_size = 188000
   },
   {
        .diameter = 195,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {194300, 194900},
        .bolt_minor = {187010, 188430},
        .internal_major = {188510, 189310},
        .internal_minor = {195000, 196290},
        .drill_size = 189000
   },
   {
        .diameter = 195,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {194500, 194900},
        .bolt_minor = {189600, 190610},
        .internal_major = {190670, 191270},
        .internal_minor = {195000, 195950},
        .drill_size = 191000
   },
   {
        .diameter = 195,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {194600, 195000},
        .bolt_minor = {190910, 191700},
        .internal_major = {191750, 192250},
        .internal_minor = {195000, 195770},
        .drill_size = 192000
   },
   {
        .diameter = 195,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {194700, 195000},
        .bolt_minor = {192220, 192800},
        .internal_major = {192840, 193210},
        .internal_minor = {195000, 195570},
        .drill_size = 193000
   },
   {
        .diameter = 200,
        .step = 8000,
        .is_default = 0,
        .bolt_major = {199200, 199900},
        .bolt_minor = {189420, 191240},
        .internal_major = {191340, 192340},
        .internal_minor = {200000, 201630},
        .drill_size = 192000
   },
   {
        .diameter = 200,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {199300, 199900},
        .bolt_minor = {192010, 193430},
        .internal_major = {193510, 194310},
        .internal_minor = {200000, 201290},
        .drill_size = 194000
   },
   {
        .diameter = 200,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {199500, 199900},
        .bolt_minor = {194600, 195610},
        .internal_major = {195670, 196270},
        .internal_minor = {200000, 200950},
        .drill_size = 196000
   },
   {
        .diameter = 200,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {199600, 200000},
        .bolt_minor = {195910, 196700},
        .internal_major = {196750, 197250},
        .internal_minor = {200000, 200770},
        .drill_size = 197000
   },
   {
        .diameter = 200,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {199700, 200000},
        .bolt_minor = {197220, 197800},
        .internal_major = {197840, 198210},
        .internal_minor = {200000, 200570},
        .drill_size = 198000
   },
   {
        .diameter = 205,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {204300, 204900},
        .bolt_minor = {197010, 198430},
        .internal_major = {198510, 199310},
        .internal_minor = {205000, 206290},
        .drill_size = 199000
   },
   {
        .diameter = 205,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {204500, 204900},
        .bolt_minor = {199600, 200610},
        .internal_major = {200670, 201270},
        .internal_minor = {205000, 205950},
        .drill_size = 201000
   },
   {
        .diameter = 205,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {204600, 205000},
        .bolt_minor = {200910, 201700},
        .internal_major = {201750, 202250},
        .internal_minor = {205000, 205770},
        .drill_size = 202000
   },
   {
        .diameter = 205,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {204700, 205000},
        .bolt_minor = {202220, 202800},
        .internal_major = {202840, 203210},
        .internal_minor = {205000, 205570},
        .drill_size = 203000
   },
   {
        .diameter = 210,
        .step = 8000,
        .is_default = 0,
        .bolt_major = {209200, 209900},
        .bolt_minor = {199420, 201240},
        .internal_major = {201340, 202340},
        .internal_minor = {210000, 211630},
        .drill_size = 202000
   },
   {
        .diameter = 210,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {209300, 209900},
        .bolt_minor = {202010, 203430},
        .internal_major = {203510, 204310},
        .internal_minor = {210000, 211290},
        .drill_size = 204000
   },
   {
        .diameter = 210,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {209500, 209900},
        .bolt_minor = {204600, 205610},
        .internal_major = {205670, 206270},
        .internal_minor = {210000, 210950},
        .drill_size = 206000
   },
   {
        .diameter = 210,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {209600, 210000},
        .bolt_minor = {205910, 206700},
        .internal_major = {206750, 207250},
        .internal_minor = {210000, 210770},
        .drill_size = 207000
   },
   {
        .diameter = 210,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {209700, 210000},
        .bolt_minor = {207220, 207800},
        .internal_major = {207840, 208210},
        .internal_minor = {210000, 210570},
        .drill_size = 208000
   },
   {
        .diameter = 215,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {214300, 214900},
        .bolt_minor = {207010, 208430},
        .internal_major = {208510, 209310},
        .internal_minor = {215000, 216290},
        .drill_size = 209000
   },
   {
        .diameter = 215,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {214500, 214900},
        .bolt_minor = {209600, 210610},
        .internal_major = {210670, 211270},
        .internal_minor = {215000, 215950},
        .drill_size = 211000
   },
   {
        .diameter = 215,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {214600, 215000},
        .bolt_minor = {210910, 211700},
        .internal_major = {211750, 212250},
        .internal_minor = {215000, 215770},
        .drill_size = 212000
   },
   {
        .diameter = 220,
        .step = 8000,
        .is_default = 0,
        .bolt_major = {219200, 219900},
        .bolt_minor = {209420, 211240},
        .internal_major = {211340, 212340},
        .internal_minor = {220000, 221630},
        .drill_size = 212000
   },
   {
        .diameter = 220,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {219300, 219900},
        .bolt_minor = {212010, 213430},
        .internal_major = {213510, 214310},
        .internal_minor = {220000, 221290},
        .drill_size = 214000
   },
   {
        .diameter = 220,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {219500, 219900},
        .bolt_minor = {214600, 215610},
        .internal_major = {215670, 216270},
        .internal_minor = {220000, 220950},
        .drill_size = 216000
   },
   {
        .diameter = 220,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {219600, 220000},
        .bolt_minor = {215910, 216700},
        .internal_major = {216750, 217250},
        .internal_minor = {220000, 220770},
        .drill_size = 217000
   },
   {
        .diameter = 220,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {219700, 220000},
        .bolt_minor = {217220, 217800},
        .internal_major = {217840, 218210},
        .internal_minor = {220000, 220570},
        .drill_size = 218000
   },
   {
        .diameter = 225,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {224500, 224900},
        .bolt_minor = {217010, 218430},
        .internal_major = {218510, 219310},
        .internal_minor = {225000, 226290},
        .drill_size = 219000
   },
   {
        .diameter = 225,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {224500, 224900},
        .bolt_minor = {219600, 220610},
        .internal_major = {220670, 221270},
        .internal_minor = {225000, 225950},
        .drill_size = 221000
   },
   {
        .diameter = 225,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {224600, 225000},
        .bolt_minor = {220910, 221700},
        .internal_major = {221750, 222250},
        .internal_minor = {225000, 225770},
        .drill_size = 222000
   },
   {
        .diameter = 225,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {224700, 225000},
        .bolt_minor = {222220, 222800},
        .internal_major = {222840, 223210},
        .internal_minor = {225000, 225570},
        .drill_size = 223000
   },
   {
        .diameter = 230,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {229300, 229900},
        .bolt_minor = {222010, 223430},
        .internal_major = {223510, 224310},
        .internal_minor = {230000, 231290},
        .drill_size = 224000
   },
   {
        .diameter = 230,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {229500, 229900},
        .bolt_minor = {224600, 225610},
        .internal_major = {225670, 226270},
        .internal_minor = {230000, 230950},
        .drill_size = 226000
   },
   {
        .diameter = 230,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {229600, 230000},
        .bolt_minor = {225910, 226700},
        .internal_major = {226750, 227250},
        .internal_minor = {230000, 230770},
        .drill_size = 227000
   },
   {
        .diameter = 230,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {229700, 230000},
        .bolt_minor = {227220, 227800},
        .internal_major = {227840, 228210},
        .internal_minor = {230000, 230570},
        .drill_size = 228000
   },
   {
        .diameter = 235,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {234300, 234900},
        .bolt_minor = {226970, 228430},
        .internal_major = {228510, 229310},
        .internal_minor = {235000, 236340},
        .drill_size = 229000
   },
   {
        .diameter = 235,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {234500, 234900},
        .bolt_minor = {229600, 230610},
        .internal_major = {230670, 231270},
        .internal_minor = {235000, 235950},
        .drill_size = 231000
   },
   {
        .diameter = 235,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {234600, 235000},
        .bolt_minor = {230910, 231700},
        .internal_major = {231750, 232250},
        .internal_minor = {235000, 235770},
        .drill_size = 232000
   },
   {
        .diameter = 240,
        .step = 8000,
        .is_default = 0,
        .bolt_major = {239200, 239900},
        .bolt_minor = {229420, 231240},
        .internal_major = {231340, 232340},
        .internal_minor = {240000, 241630},
        .drill_size = 232000
   },
   {
        .diameter = 240,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {239300, 239900},
        .bolt_minor = {232010, 233430},
        .internal_major = {233510, 234310},
        .internal_minor = {240000, 241290},
        .drill_size = 234000
   },
   {
        .diameter = 240,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {239500, 239900},
        .bolt_minor = {234600, 235610},
        .internal_major = {235670, 236270},
        .internal_minor = {240000, 240950},
        .drill_size = 236000
   },
   {
        .diameter = 240,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {239600, 240000},
        .bolt_minor = {235910, 236700},
        .internal_major = {236750, 237250},
        .internal_minor = {240000, 240770},
        .drill_size = 237000
   },
   {
        .diameter = 240,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {239700, 240000},
        .bolt_minor = {237220, 237800},
        .internal_major = {237840, 238210},
        .internal_minor = {240000, 240570},
        .drill_size = 238000
   },
   {
        .diameter = 245,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {244300, 244900},
        .bolt_minor = {237010, 238430},
        .internal_major = {238510, 239310},
        .internal_minor = {245000, 246290},
        .drill_size = 239000
   },
   {
        .diameter = 245,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {244500, 244900},
        .bolt_minor = {239600, 240610},
        .internal_major = {240670, 241270},
        .internal_minor = {245000, 245950},
        .drill_size = 241000
   },
   {
        .diameter = 245,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {244600, 245000},
        .bolt_minor = {240910, 241700},
        .internal_major = {241750, 242250},
        .internal_minor = {245000, 245770},
        .drill_size = 242000
   },
   {
        .diameter = 245,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {244700, 245000},
        .bolt_minor = {242220, 242800},
        .internal_major = {242840, 243210},
        .internal_minor = {245000, 245570},
        .drill_size = 243000
   },
   {
        .diameter = 250,
        .step = 8000,
        .is_default = 0,
        .bolt_major = {249200, 249900},
        .bolt_minor = {239420, 241240},
        .internal_major = {241340, 242340},
        .internal_minor = {250000, 251630},
        .drill_size = 242000
   },
   {
        .diameter = 250,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {249300, 249900},
        .bolt_minor = {242010, 243430},
        .internal_major = {243510, 244310},
        .internal_minor = {250000, 251290},
        .drill_size = 244000
   },
   {
        .diameter = 250,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {249500, 249900},
        .bolt_minor = {244600, 245610},
        .internal_major = {245670, 246270},
        .internal_minor = {250000, 250950},
        .drill_size = 246000
   },
   {
        .diameter = 250,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {249600, 250000},
        .bolt_minor = {245910, 246700},
        .internal_major = {246750, 247250},
        .internal_minor = {250000, 250770},
        .drill_size = 247000
   },
   {
        .diameter = 250,
        .step = 2000,
        .is_default = 0,
        .bolt_major = {249700, 250000},
        .bolt_minor = {247220, 247800},
        .internal_major = {247840, 248210},
        .internal_minor = {250000, 250570},
        .drill_size = 248000
   },
   {
        .diameter = 255,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {254300, 254900},
        .bolt_minor = {246970, 248430},
        .internal_major = {248510, 249310},
        .internal_minor = {255000, 256339},
        .drill_size = 249000
   },
   {
        .diameter = 255,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {254500, 254900},
        .bolt_minor = {249600, 250610},
        .internal_major = {250670, 251270},
        .internal_minor = {255000, 255950},
        .drill_size = 251000
   },
   {
        .diameter = 255,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {254600, 255000},
        .bolt_minor = {250910, 251700},
        .internal_major = {251750, 252250},
        .internal_minor = {255000, 255770},
        .drill_size = 252000
   },
   {
        .diameter = 260,
        .step = 8000,
        .is_default = 0,
        .bolt_major = {259200, 259899},
        .bolt_minor = {249420, 251240},
        .internal_major = {251340, 252340},
        .internal_minor = {260000, 261630},
        .drill_size = 252000
   },
   {
        .diameter = 260,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {259300, 259899},
        .bolt_minor = {252010, 253430},
        .internal_major = {253510, 254310},
        .internal_minor = {260000, 261290},
        .drill_size = 254000
   },
   {
        .diameter = 260,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {259500, 259899},
        .bolt_minor = {254600, 255610},
        .internal_major = {255670, 256269},
        .internal_minor = {260000, 260950},
        .drill_size = 256000
   },
   {
        .diameter = 260,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {259600, 260000},
        .bolt_minor = {255910, 256700},
        .internal_major = {256750, 257250},
        .internal_minor = {260000, 260769},
        .drill_size = 257000
   },
   {
        .diameter = 265,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {264300, 264900},
        .bolt_minor = {257010, 258430},
        .internal_major = {258510, 259310},
        .internal_minor = {265000, 266290},
        .drill_size = 259000
   },
   {
        .diameter = 265,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {264500, 264900},
        .bolt_minor = {259600, 260610},
        .internal_major = {260670, 261269},
        .internal_minor = {265000, 265950},
        .drill_size = 261000
   },
   {
        .diameter = 265,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {264600, 265000},
        .bolt_minor = {260910, 261700},
        .internal_major = {261750, 262250},
        .internal_minor = {265000, 265770},
        .drill_size = 262000
   },
   {
        .diameter = 270,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {269300, 269900},
        .bolt_minor = {262010, 263430},
        .internal_major = {263510, 264310},
        .internal_minor = {270000, 271290},
        .drill_size = 264000
   },
   {
        .diameter = 270,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {269500, 269900},
        .bolt_minor = {264600, 265610},
        .internal_major = {265670, 266270},
        .internal_minor = {270000, 270950},
        .drill_size = 266000
   },
   {
        .diameter = 270,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {269600, 270000},
        .bolt_minor = {265910, 266700},
        .internal_major = {266750, 267250},
        .internal_minor = {270000, 270770},
        .drill_size = 267000
   },
   {
        .diameter = 275,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {274300, 274900},
        .bolt_minor = {267010, 268430},
        .internal_major = {268510, 269310},
        .internal_minor = {275000, 276290},
        .drill_size = 269000
   },
   {
        .diameter = 275,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {274500, 274900},
        .bolt_minor = {269600, 270610},
        .internal_major = {270670, 271270},
        .internal_minor = {275000, 275950},
        .drill_size = 271000
   },
   {
        .diameter = 275,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {274600, 275000},
        .bolt_minor = {270910, 271700},
        .internal_major = {271750, 272250},
        .internal_minor = {275000, 275770},
        .drill_size = 272000
   },
   {
        .diameter = 280,
        .step = 8000,
        .is_default = 0,
        .bolt_major = {279200, 279900},
        .bolt_minor = {269420, 271240},
        .internal_major = {271340, 272340},
        .internal_minor = {280000, 281630},
        .drill_size = 272000
   },
   {
        .diameter = 280,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {279300, 279900},
        .bolt_minor = {272010, 273430},
        .internal_major = {273510, 274310},
        .internal_minor = {280000, 281290},
        .drill_size = 274000
   },
   {
        .diameter = 280,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {279500, 279900},
        .bolt_minor = {274600, 275610},
        .internal_major = {275670, 276270},
        .internal_minor = {280000, 280950},
        .drill_size = 276000
   },
   {
        .diameter = 280,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {279600, 280000},
        .bolt_minor = {275910, 276700},
        .internal_major = {276750, 277250},
        .internal_minor = {280000, 280770},
        .drill_size = 277000
   },
   {
        .diameter = 285,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {284300, 284900},
        .bolt_minor = {277010, 278430},
        .internal_major = {278510, 279310},
        .internal_minor = {285000, 286290},
        .drill_size = 279000
   },
   {
        .diameter = 285,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {284500, 284900},
        .bolt_minor = {279600, 280610},
        .internal_major = {280670, 281270},
        .internal_minor = {285000, 285950},
        .drill_size = 281000
   },
   {
        .diameter = 285,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {284600, 285000},
        .bolt_minor = {280910, 281700},
        .internal_major = {281750, 282250},
        .internal_minor = {285000, 285770},
        .drill_size = 282000
   },
   {
        .diameter = 290,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {289300, 289900},
        .bolt_minor = {282010, 283430},
        .internal_major = {283510, 284310},
        .internal_minor = {290000, 291290},
        .drill_size = 284000
   },
   {
        .diameter = 290,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {289500, 289900},
        .bolt_minor = {284600, 285610},
        .internal_major = {285670, 286270},
        .internal_minor = {290000, 290950},
        .drill_size = 286000
   },
   {
        .diameter = 290,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {289600, 290000},
        .bolt_minor = {285910, 286700},
        .internal_major = {286750, 287250},
        .internal_minor = {290000, 290770},
        .drill_size = 287000
   },
   {
        .diameter = 295,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {294300, 294900},
        .bolt_minor = {287010, 288430},
        .internal_major = {288510, 289310},
        .internal_minor = {295000, 296290},
        .drill_size = 289000
   },
   {
        .diameter = 295,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {294500, 294900},
        .bolt_minor = {289600, 290610},
        .internal_major = {290670, 291270},
        .internal_minor = {295000, 295950},
        .drill_size = 291000
   },
   {
        .diameter = 295,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {294600, 295000},
        .bolt_minor = {290910, 291700},
        .internal_major = {291750, 292250},
        .internal_minor = {295000, 295770},
        .drill_size = 292000
   },
   {
        .diameter = 300,
        .step = 8000,
        .is_default = 0,
        .bolt_major = {299200, 299900},
        .bolt_minor = {289420, 291240},
        .internal_major = {291340, 292340},
        .internal_minor = {300000, 301630},
        .drill_size = 292000
   },
   {
        .diameter = 300,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {299300, 299900},
        .bolt_minor = {292010, 293430},
        .internal_major = {293510, 294310},
        .internal_minor = {300000, 301290},
        .drill_size = 294000
   },
   {
        .diameter = 300,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {299500, 299900},
        .bolt_minor = {294600, 295610},
        .internal_major = {295670, 296270},
        .internal_minor = {300000, 300950},
        .drill_size = 296000
   },
   {
        .diameter = 300,
        .step = 3000,
        .is_default = 0,
        .bolt_major = {299600, 300000},
        .bolt_minor = {295910, 296700},
        .internal_major = {296750, 297250},
        .internal_minor = {300000, 300770},
        .drill_size = 297000
   },
   {
        .diameter = 310,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {309300, 309900},
        .bolt_minor = {302010, 303430},
        .internal_major = {303510, 304310},
        .internal_minor = {310000, 311290},
        .drill_size = 304000
   },
   {
        .diameter = 310,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {309500, 309900},
        .bolt_minor = {304600, 305610},
        .internal_major = {305670, 306270},
        .internal_minor = {310000, 310950},
        .drill_size = 306000
   },
   {
        .diameter = 320,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {319300, 319900},
        .bolt_minor = {312010, 313430},
        .internal_major = {313510, 314310},
        .internal_minor = {320000, 321290},
        .drill_size = 314000
   },
   {
        .diameter = 320,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {319500, 319900},
        .bolt_minor = {314600, 315610},
        .internal_major = {315670, 316270},
        .internal_minor = {320000, 320950},
        .drill_size = 316000
   },
   {
        .diameter = 330,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {329300, 329900},
        .bolt_minor = {322010, 323430},
        .internal_major = {323510, 324310},
        .internal_minor = {330000, 331290},
        .drill_size = 324000
   },
   {
        .diameter = 330,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {329500, 329900},
        .bolt_minor = {324600, 325610},
        .internal_major = {325670, 326270},
        .internal_minor = {330000, 330950},
        .drill_size = 326000
   },
   {
        .diameter = 340,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {339300, 339900},
        .bolt_minor = {332010, 333430},
        .internal_major = {333510, 334310},
        .internal_minor = {340000, 341290},
        .drill_size = 334000
   },
   {
        .diameter = 340,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {339500, 339900},
        .bolt_minor = {334600, 335610},
        .internal_major = {335670, 336270},
        .internal_minor = {340000, 340950},
        .drill_size = 336000
   },
   {
        .diameter = 350,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {349300, 349900},
        .bolt_minor = {342010, 343430},
        .internal_major = {343510, 344310},
        .internal_minor = {350000, 351290},
        .drill_size = 344000
   },
   {
        .diameter = 350,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {349500, 349900},
        .bolt_minor = {344600, 345610},
        .internal_major = {345670, 346270},
        .internal_minor = {350000, 350950},
        .drill_size = 346000
   },
   {
        .diameter = 360,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {359300, 359900},
        .bolt_minor = {351970, 353430},
        .internal_major = {353510, 354310},
        .internal_minor = {360000, 361340},
        .drill_size = 354000
   },
   {
        .diameter = 360,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {359500, 359900},
        .bolt_minor = {354580, 355610},
        .internal_major = {355670, 356270},
        .internal_minor = {360000, 360980},
        .drill_size = 356000
   },
   {
        .diameter = 370,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {369300, 369900},
        .bolt_minor = {361970, 363430},
        .internal_major = {363510, 364310},
        .internal_minor = {370000, 371340},
        .drill_size = 364000
   },
   {
        .diameter = 370,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {369500, 369900},
        .bolt_minor = {364580, 365610},
        .internal_major = {365670, 366270},
        .internal_minor = {370000, 370980},
        .drill_size = 366000
   },
   {
        .diameter = 380,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {379300, 379900},
        .bolt_minor = {371970, 373430},
        .internal_major = {373510, 374310},
        .internal_minor = {380000, 381340},
        .drill_size = 374000
   },
   {
        .diameter = 380,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {379500, 379900},
        .bolt_minor = {374580, 375610},
        .internal_major = {375670, 376270},
        .internal_minor = {380000, 380980},
        .drill_size = 376000
   },
   {
        .diameter = 390,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {389300, 389900},
        .bolt_minor = {381970, 383430},
        .internal_major = {383510, 384310},
        .internal_minor = {390000, 391340},
        .drill_size = 384000
   },
   {
        .diameter = 390,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {389500, 389900},
        .bolt_minor = {384580, 385610},
        .internal_major = {385670, 386270},
        .internal_minor = {390000, 390980},
        .drill_size = 386000
   },
   {
        .diameter = 400,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {399300, 399900},
        .bolt_minor = {391970, 393430},
        .internal_major = {393510, 394310},
        .internal_minor = {400000, 401340},
        .drill_size = 394000
   },
   {
        .diameter = 400,
        .step = 4000,
        .is_default = 0,
        .bolt_major = {399500, 399900},
        .bolt_minor = {394580, 395610},
        .internal_major = {395670, 396270},
        .internal_minor = {400000, 400980},
        .drill_size = 396000
   },
   {
        .diameter = 410,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {409300, 409900},
        .bolt_minor = {401970, 403430},
        .internal_major = {403510, 404310},
        .internal_minor = {410000, 411340},
        .drill_size = 404000
   },
   {
        .diameter = 420,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {419300, 419900},
        .bolt_minor = {411970, 413430},
        .internal_major = {413510, 414310},
        .internal_minor = {420000, 421340},
        .drill_size = 414000
   },
   {
        .diameter = 430,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {429300, 429900},
        .bolt_minor = {421970, 423430},
        .internal_major = {423510, 424310},
        .internal_minor = {430000, 431340},
        .drill_size = 424000
   },
   {
        .diameter = 440,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {439300, 439900},
        .bolt_minor = {431970, 433430},
        .internal_major = {433510, 434310},
        .internal_minor = {440000, 441340},
        .drill_size = 434000
   },
   {
        .diameter = 450,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {449300, 449900},
        .bolt_minor = {441970, 443430},
        .internal_major = {443510, 444310},
        .internal_minor = {450000, 451340},
        .drill_size = 444000
   },
   {
        .diameter = 460,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {459300, 459900},
        .bolt_minor = {451970, 453430},
        .internal_major = {453510, 454310},
        .internal_minor = {460000, 461340},
        .drill_size = 454000
   },
   {
        .diameter = 470,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {469300, 469900},
        .bolt_minor = {461970, 463430},
        .internal_major = {463510, 464310},
        .internal_minor = {470000, 471340},
        .drill_size = 464000
   },
   {
        .diameter = 480,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {479300, 479900},
        .bolt_minor = {471970, 473430},
        .internal_major = {473510, 474310},
        .internal_minor = {480000, 481340},
        .drill_size = 474000
   },
   {
        .diameter = 490,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {489300, 489900},
        .bolt_minor = {481970, 483430},
        .internal_major = {483510, 484310},
        .internal_minor = {490000, 491340},
        .drill_size = 484000
   },
   {
        .diameter = 500,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {499300, 499900},
        .bolt_minor = {491970, 493430},
        .internal_major = {493510, 494310},
        .internal_minor = {500000, 501340},
        .drill_size = 494000
   },
   {
        .diameter = 510,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {509300, 509900},
        .bolt_minor = {501970, 503430},
        .internal_major = {503510, 504310},
        .internal_minor = {510000, 511340},
        .drill_size = 504000
   },
   {
        .diameter = 520,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {519299, 519900},
        .bolt_minor = {511970, 513429},
        .internal_major = {513510, 514309},
        .internal_minor = {520000, 521340},
        .drill_size = 514000
   },
   {
        .diameter = 530,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {529300, 529900},
        .bolt_minor = {521970, 523429},
        .internal_major = {523510, 524310},
        .internal_minor = {530000, 531340},
        .drill_size = 524000
   },
   {
        .diameter = 540,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {539300, 539900},
        .bolt_minor = {531970, 533430},
        .internal_major = {533510, 534310},
        .internal_minor = {540000, 541340},
        .drill_size = 534000
   },
   {
        .diameter = 550,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {549300, 549900},
        .bolt_minor = {541970, 543430},
        .internal_major = {543510, 544310},
        .internal_minor = {550000, 551340},
        .drill_size = 544000
   },
   {
        .diameter = 560,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {559300, 559900},
        .bolt_minor = {551970, 553430},
        .internal_major = {553510, 554310},
        .internal_minor = {560000, 561340},
        .drill_size = 554000
   },
   {
        .diameter = 570,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {569300, 569900},
        .bolt_minor = {561970, 563430},
        .internal_major = {563510, 564310},
        .internal_minor = {570000, 571340},
        .drill_size = 564000
   },
   {
        .diameter = 580,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {579300, 579900},
        .bolt_minor = {571970, 573430},
        .internal_major = {573510, 574310},
        .internal_minor = {580000, 581340},
        .drill_size = 574000
   },
   {
        .diameter = 590,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {589300, 589900},
        .bolt_minor = {581970, 583430},
        .internal_major = {583510, 584310},
        .internal_minor = {590000, 591340},
        .drill_size = 584000
   },
   {
        .diameter = 600,
        .step = 6000,
        .is_default = 0,
        .bolt_major = {599300, 599900},
        .bolt_minor = {591970, 593430},
        .internal_major = {593510, 594310},
        .internal_minor = {600000, 601340},
        .drill_size = 594000
   },
   {
        .diameter = 0
   }
};

