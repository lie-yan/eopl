This piece of code meets the requirements of Exercise 3.7, 3.8, 3.9, 3.10, 3.11, and 3.12.

Since we use std::shared_ptr to represent `Value`, it is natural to ask whether there is any cyclic reference leading to memory leakage. Is it possible to prove that std::shared_ptr's here never form a cycle.