#!/usr/bin/python3

import time
from argparse import ArgumentParser
from typing import Dict, List

import numpy as np
from mpi4py import MPI


def parse_arguments():
    parser = ArgumentParser()
    parser.add_argument("--aut_size", dest="aut_size", type=int, default=16)
    parser.add_argument("--rule_number", dest="rule_number", type=int, default=110)
    parser.add_argument("--root", dest="root", type=int, default=0)
    parser.add_argument("--n_steps", dest="n_steps", type=int, default=10)
    parser.add_argument("--show", dest="show", type=bool, default=False)
    return parser.parse_args()


class CellularAutomaton:
    def __init__(
            self, aut_size: int = 16, rule_number: int = 110, root: int = 0, n_steps: int = 10
    ):
        self.N = aut_size
        self.rule = self.get_rule(rule_number=rule_number)
        self.root = root
        self.n_steps = n_steps

    def get_rule(self, rule_number: int) -> Dict[tuple, int]:
        """Rule."""
        rule = {}
        bin_format = '{0:08b}'.format(rule_number)[::-1]
        for i, c in enumerate(bin_format):
            pattern = tuple(map(int, list('{0:03b}'.format(i))))
            rule[pattern] = int(c)
        return rule

    def show_state(self, state: List[int]):
        """Show one epoch."""
        line = list(map(lambda x: " " if x == 1 else "\u2588", state))
        print("".join(line), end="")
        print("|")

    def init_first_state(self, chunk_size: int) -> np.array:
        """Init first random state."""
        # +2 for ghost cells on edges
        state = np.random.randint(0, 2, chunk_size, np.int32)
        state[0] = -1  # ghost cell
        state[-1] = -1  # ghost cell
        return state

    def send_recv(self, comm, state: np.array, rank: int, size: int):
        """Send-receive."""

        # next rank
        rank_next = rank + 1
        if rank_next == size:
            rank_next = 0
        # rank previous
        rank_prev = rank - 1
        if rank_prev == -1:
            rank_prev = size - 1

        # send to previous
        comm.Send([state[1:2], 1, MPI.INT], dest=rank_prev, tag=0)
        # send to next
        comm.Send([state[state.size-2:state.size-1], 1, MPI.INT], dest=rank_next, tag=0)

        # receive from previous
        comm.Recv([state[0:1], 1, MPI.INT], source=rank_prev, tag=0)
        # receive from next
        comm.Recv([state[state.size-1:state.size], 1, MPI.INT], source=rank_next, tag=0)

    def update_state(self, state: np.array):
        """Update state."""

        state_new = np.zeros_like(state)

        for i in range(1, state.size - 1):
            state_new[i] = self.rule[(state[i - 1], state[i], state[i + 1])]

        return state_new

    def run(self, show: bool = False):
        """Run automaton."""

        time_start = time.time()

        comm = MPI.COMM_WORLD
        size = comm.Get_size()
        rank = comm.Get_rank()

        chunk_size = self.N // size
        state = self.init_first_state(chunk_size=chunk_size + 2)

        state_full = np.zeros((size, chunk_size), dtype=np.int32)

        for _ in range(self.n_steps):
            self.send_recv(comm=comm, state=state, rank=rank, size=size)
            state = self.update_state(state=state)
            comm.Gather(state[1:-1], state_full, root=self.root)
            if show and rank == self.root:
                self.show_state(state=state_full.flatten())

        elapsed_time = time.time() - time_start
        elapsed_time_list = comm.gather(elapsed_time, root=self.root)
        if rank == self.root:
            print(f"elapsed time with {size} processes: {np.round(np.mean(elapsed_time_list), 2)} s")


if __name__ == "__main__":

    args = parse_arguments()
    ca = CellularAutomaton(
        aut_size=args.aut_size,
        rule_number=args.rule_number,
        root=args.root,
        n_steps=args.n_steps
    )
    ca.run(args.show)
