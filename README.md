Impinj Indy ITK releases
========================

This repository tracks Indy ITK releases as released by Impinj and [downloaded from their support
portal](https://support.impinj.com/hc/en-us/articles/202755788) (valid login required).

I had to remove all Makefiles as they're heavily guarded trade secrets :-), since unfortunately they contained following
licenses:

```
# /*
#  *****************************************************************************
#  *                                                                           *
#  *              IMPINJ CONFIDENTIAL AND PROPRIETARY                          *
#  *                                                                           *
#  * This source code is the sole property of Impinj, Inc. Reproduction or     *
#  * utilization of this source code in whole or in part is forbidden without  *
#  * the prior written consent of Impinj, Inc.                                 *
#  *                                                                           *
#  * (c) Copyright Impinj, Inc. 2013-2014. All rights reserved.                *
#  *                                                                           *
#  *****************************************************************************/
```

I've also removed Documentation folder, because it either contains non-redistributable documentation or documentation
which can be found [online already](https://developer.impinj.com/indy/itk).
