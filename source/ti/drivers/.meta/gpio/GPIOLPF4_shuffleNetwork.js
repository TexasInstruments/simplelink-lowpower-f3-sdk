/*
 * Copyright (c) 2025 Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

function _computeShuffleNetwork(routes, size) {

    let cfgBits = new Array(0);

    /* Create deep copy of the routes since they will be modified by this function */
    let routesCopy = new Array(routes.length);

    for (let i = 0; i < routes.length; i++)
    {
        let route = routes[i];
        routesCopy[i] = {
            input: route.input,
            output: route.output,
            inputFork: undefined, /* Used by the algorithm to reference the route connected to the same input switch */
            outputFork: undefined /* Used by the algorithm to reference the route connected to the same output switch */
        };
    }

    /* Helper function to calculate the number of configuration bits needed for
     * a shuffle network of the specified size.
     */
    function numberOfCfgBits(size) {
        if (size == 0 || size == 1) {
            return 0;
        }
        else if (size == 2) {
            /* One 2x2 switch, with one configuration bit */
            return 1;
        }
        else {
            /* Input stage: floor(size / 2) bits
             * Output stage: floor(size / 2) bits
             * Upper layer size: floor(size / 2)
             * Lower layer size: ceil(size / 2)
             * Recursively determine number of configuration bits for upper and
             * lower layers.
             */
            if (size % 2 == 0) {
                /* Even size. The two middle layers have the same size. */
                return 2 * (Math.floor(size / 2)) +
                    2 * numberOfCfgBits(Math.floor(size / 2));
            }
            else
            {
                return 2 * (Math.floor(size / 2)) +
                    numberOfCfgBits(Math.floor(size / 2)) +
                    numberOfCfgBits(Math.ceil(size / 2));
            }
        }
    }

    /* Main function used to recursively compute the shuffle network.
     * The base cases of the recursion are:
     * - No routes to compute network for -> cfg bits do not matter.
     * - Size < 2: No configuration bits.
     * - Size == 2: Trivial case with one configuration bits.
     *
     * For non-base cases each route is assigned to either the upper middle
     * network or the lower middle network. And the configuration bits for the
     * input/output switches are configured accordingly and then the middle
     * networks are computed recursively.
     */
    function computeShuffleNetworkRecursively(routes, size) {
        /* Handle base cases of the recursion */
        if (routes.length == 0) {
            /* No routes in this network, no need to do recursion, just fill in
             * configuration bits with 0s.
             */
            cfgBits.push(... new Array(numberOfCfgBits(size)).fill(0));
            return;
        }
        else if (size == 0 || size == 1) {
            /* No configuration bits for 0x0 and 1x1 networks */
            return;
        }
        else if (size == 2) {
            /* 2x2 network has one configuration bit.
             * The input and output numbers are 1 bit wide (0 or 1).
             * If the input and output numbers are different (XOR is 1), then
             * the 2x2 butterfly needs to cross the input and output, meaning
             * the configuration bit should be set.
             *
             * */
            cfgBits.push(routes[0].output ^ routes[0].input);
            return;
        }

        /* Maintain two arrays:
         * - Routes assigned to upper middle network.
         * - Routes assigned to lower middle network.
         */
        let upperRoutes = [];
        let lowerRoutes = [];

        /* For odd sized networks, the routes (if they exist) using the last
         * input or output must be evaluated first, since they must use
         * the lower middle network. These variables are used to keep track of
         * those routes.
         */
        let lastInputRoute = undefined;
        let lastOutputRoute = undefined;

        /* The set of routes can be thought of as a set of chains, the "link"
         * between two routes in a chain is that they share the same input or
         * output switch. Two routes sharing an input/output switch must be
         * connected to different middle layer networks. That is, one must be
         * connected to the upper layer and the other must be connected to the
         * lower layer. To compute the network, the chains are therefore
         * traversed to ensure that this requirement is met by alternating the
         * middle layer network used for each route in the chain.
         * Two chains are independent of each other since they don't share any
         * input/output switches.
         *
         * But first the chains must be constructed. This is done by traversing
         * all routes, and connecting them to existing chains if they share
         * an input or output switch, or creating a new chain if they don't.
         * A route may be connected to two chains if they share input switch
         * with one chain and output switch with another. In that case the two
         * chains are merged into one.
         * A route might also close the loop of a chain, but this is not
         * desired, so in that case we just connect the route to one end of the
         * chain only.
         *
         * A chain is a Map mapping chain ends switch indexes (key) to routes
         * (value). It will always have two keys, one for each end of the chain,
         * except during the construction of the chains where it temporarily may
         * have fewer keys.
         *
         * A Map object (chainEnds) is used to keep track of all the ends of all
         * the chains. It has the following structure. Each key is the switch
         * index of the chain end and the value is the chain object (Map) with
         * that end.
         *
         * Switch indexes < 128 are input switch indexes (input switch index =
         * switch index), and switch indexes >= 128 are output switch indexes
         * (output switch index = switch index - 128).
         */
        let chainEnds = new Map();

        /* Traverse through all routes. */
        for (let route of routes)
        {
            /* The input and output switch indexes are derived from the input
             * and output port numbers. The input port number is divided by 2 to
             * get the switch index for the input switch, and the output port
             * number is divided by 2 and 128 is added to get the switch index
             * for the output switch.
             */
            let inputSwitch = route.input >> 1;
            let outputSwitch = (route.output >> 1) + 128;

            /* Get the chains connected to the input and output ports of the
             * route.
             */
            let inputChain = chainEnds.get(inputSwitch);
            let outputChain = chainEnds.get(outputSwitch);

            if (inputChain === undefined && outputChain === undefined)
            {
                /* No chain is connected to the same input/output switch.
                 * Create new chain.
                 */
                let chain = new Map();

                /* Populate chain map. */
                chain.set(inputSwitch, route);
                chain.set(outputSwitch, route);

                /* Add the new chain to chainEnds. */
                chainEnds.set(inputSwitch, chain);
                chainEnds.set(outputSwitch, chain);
            }
            else
            {
                /* Route should be added to either the input chain or output
                 * chain, or both.
                 */

                if (inputChain !== undefined)
                {
                    /* Append to input chain */
                    let inputFork = inputChain.get(inputSwitch);
                    inputFork.inputFork = route; /* Connect the route to the end of the input chain. */
                    route.inputFork = inputFork; /* Connect the end of the input chain to the route. */
                    inputChain.delete(inputSwitch); /* Remove the old end of the input chain. */
                    inputChain.set(outputSwitch, route); /* The new end of the input chain is the output of the route. */
                    chainEnds.delete(inputSwitch); /* Remove the old end of the object of chain ends. */
                    chainEnds.set(outputSwitch, inputChain); /* The new end of the chain is the output of the route. */
                }
                if (outputChain !== undefined)
                {
                    if (outputChain === inputChain)
                    {
                        /* Do not append to output chain, since it is the same
                         * as the input chain and we do not want to close
                         * the chain.
                         */
                    }
                    else
                    {
                        /* Append to output chain. */
                        let outputFork = outputChain.get(outputSwitch);
                        outputFork.outputFork = route; /* Connect the route to the end of the output chain. */
                        route.outputFork = outputFork; /* Connect the end of the output chain to the route. */
                        outputChain.delete(outputSwitch); /* Remove the old end of the output chain. */
                        outputChain.set(inputSwitch, route); /* The new end of the output chain is the input of the route. */
                        chainEnds.delete(outputSwitch); /* Remove the old end of the object of chain ends. */
                        chainEnds.set(inputSwitch, outputChain); /* The new end of the chain is the input of the route. */

                        if (inputChain !== undefined && outputChain !== undefined)
                        {
                            /* Merge the two chains (keep input chain). */

                            /* Delete the chain ends associated with this route,
                             * since they are not actual ends of the chain.
                             * After this, the input chain and output chain will
                             * only have one chain end each.
                             */
                            chainEnds.delete(outputSwitch);
                            inputChain.delete(outputSwitch);
                            outputChain.delete(inputSwitch);
                            chainEnds.delete(inputSwitch);

                            /* The output chain end should now be 2nd chain end
                             * of the input chain since that is the chain we
                             * will keep.
                             * This overwrites the old value.
                             */
                            let outputChainEndIndex = outputChain.keys().next().value;
                            chainEnds.set(outputChainEndIndex, inputChain);

                            /* Add the end of the output chain to the input chain. */
                            inputChain.set(outputChainEndIndex, outputChain.get(outputChainEndIndex));
                        }
                    }
                }
            }
        }

        /* Reference to the current route being processed. */
        let currentRoute;

        /* The configuration bits for the network's input and output stages. */
        let cfg =
        {
            inBits: new Array(Math.ceil(size / 2)).fill(0),
            outBits: new Array(Math.ceil(size / 2)).fill(0)
        };

        let middleNetwork; /* 0 for upper, 1 for lower */

        /* Determine the routes connected to the last input and/or output. If
         * network size is odd.
         * Note, the last routes connected to the last input or output are
         * guaranteed to be at the end of a chain since they do not have an
         * input/output switch. They are connected directly to the lower middle
         * network.
         */
        if (size % 2 == 1) {
            let inputSwitchIndex = size >> 1;
            let outputSwitchIndex = 128 + inputSwitchIndex;
            let lastInputChain = chainEnds.get(inputSwitchIndex);
            let lastOutputChain = chainEnds.get(outputSwitchIndex);
            chainEnds.delete(inputSwitchIndex);
            chainEnds.delete(outputSwitchIndex);
            if (lastInputChain !== undefined) {
                lastInputRoute = lastInputChain.get(inputSwitchIndex);
            }
            /* Only set the lastOutputRoute if it exists and is not part of the
             * same chain as lastInputRoute.
             */
            if ((lastOutputChain !== undefined) && (lastOutputChain !== lastInputChain)) {
                lastOutputRoute = lastOutputChain.get(outputSwitchIndex);
            }
        }

        /* Traverse the chains and process each route in each chain. */
        while ((chainEnds.size > 0) || (lastInputRoute !== undefined) || (lastOutputRoute !== undefined)) {
            /* Handle the last input and last output routes first. */
            if (lastInputRoute !== undefined) {
                currentRoute = lastInputRoute;
                lastInputRoute = undefined;
            }
            else if (lastOutputRoute !== undefined) {
                currentRoute = lastOutputRoute;
                lastOutputRoute = undefined;
            }
            else {
                /* Get the first chain end, and use it as the current route.
                 * And get the route at the end of the chain. And then delete
                 * the chain end so we do not process it again.
                 */
                let chainEndIndex = chainEnds.keys().next().value;
                currentRoute = chainEnds.get(chainEndIndex).get(chainEndIndex);
                chainEnds.delete(chainEndIndex);
            }

            /* Since this is the start of the chain, there is no fork, so
             * we can select either of the upper or lower middle network, except
             * for the last input/output which has to select the lower middle
             * network. So for simplicity the lower middle network is always
             * used.
             */
            middleNetwork = 1;

            /* Loop until end of chain is reached. */
            while (currentRoute !== undefined) {
                /* Cache input and output since they will be updated for the
                 * middle network layers.
                 */
                let input = currentRoute.input;
                let output = currentRoute.output;

                /* Set input configuration bit */
                cfg.inBits[Math.floor(input / 2)] = (input % 2) ^ middleNetwork;
                /* Set output configuration bit */
                cfg.outBits[Math.floor(output / 2)] = (output % 2) ^ middleNetwork;

                /* Rebase indexes for the route.
                 * floor(input/output port index / 2) will give the correct new
                 * input/output port index locally for both upper and lower
                 * networks.
                 */
                currentRoute.input = Math.floor(input / 2);
                currentRoute.output = Math.floor(output / 2);

                /* Assign route to either upper or lower network. */
                if (middleNetwork == 0) {
                    upperRoutes.push(currentRoute);
                }
                else {
                    lowerRoutes.push(currentRoute);
                }

                /* Use alternate network for the next route in the chain. */
                middleNetwork = middleNetwork ^ 1;

                /* Since we are starting from one end of the chain, and
                 * shortening the chain as we go along, we know that there is
                 * not an input AND an output fork at the same time.
                 */
                if (currentRoute.inputFork !== undefined) {
                    /* If the current route has an input fork, then we need to
                     * process the forked route next.
                     */
                    let nextRoute = currentRoute.inputFork;
                    currentRoute.inputFork = undefined; /* Clear the fork to prevent reprocessing. */
                    nextRoute.inputFork = undefined; /* Clear the fork to prevent reprocessing. */
                    currentRoute = nextRoute;
                }
                else if (currentRoute.outputFork !== undefined)
                {
                    /* If the current route has an output fork, then we need to
                     * process the forked route next.
                     */
                    let nextRoute = currentRoute.outputFork;
                    currentRoute.outputFork = undefined; /* Clear the fork to prevent reprocessing. */
                    nextRoute.outputFork = undefined; /* Clear the fork to prevent reprocessing. */
                    currentRoute = nextRoute;
                }
                else
                {
                    /* The end of the chain has been reached. Remove the end
                     * from chainEnds.
                     */
                    chainEnds.delete(input >> 1);
                    chainEnds.delete((output >> 1)  + 128);

                    break; /* Exit the loop iterating through each route in chain. */
                }
            }
        }

        /* Output configuration bits for input stage. */
        cfgBits.push(...cfg.inBits.slice(0, Math.floor(size / 2)));
        /* Output configuration bits for output stage. */
        cfgBits.push(...cfg.outBits.slice(0, Math.floor(size / 2)));

        /* Recurse upper middle layer, then lower middle layer. */
        computeShuffleNetworkRecursively(upperRoutes, Math.floor(size / 2));
        computeShuffleNetworkRecursively(lowerRoutes, Math.ceil(size / 2));
    }

    computeShuffleNetworkRecursively(routesCopy, size);

    if (cfgBits.length != numberOfCfgBits(size)) {
        throw new Error(`Configuration bits length mismatch: expected ${numberOfCfgBits(size)}, got ${cfgBits.length}`);
    }

    /* Allocate array of 32-bit integers to hold the shuffle network
     * configuration bits.
    */
    let shuffleNetworkConfig = new Array(Math.ceil(cfgBits.length / 32)).fill(0);

    /* Copy the configuration bits into the array */
    cfgBits.forEach((bit, index) => shuffleNetworkConfig[Math.floor(index / 32)] |= (bit << (index % 32)));

    for (let i = 0; i < shuffleNetworkConfig.length; i++) {
        /* Convert to unsigned 32-bit integer, by using an unsigned right
         * shift operation.
         */
        shuffleNetworkConfig[i] = shuffleNetworkConfig[i] >>> 0;
    }

    return shuffleNetworkConfig;
}


/*
 *  ======== exports ========
 *  Export functions
 */
exports._computeShuffleNetwork = _computeShuffleNetwork;