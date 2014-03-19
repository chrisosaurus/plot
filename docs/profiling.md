Profiling
=========
Various notes from some early profiling, not expected to be useful to anyone but me.


Finding potential hotspots with valgrind callgrind
--------------------------------------------------

    valgrind --tool=callgrind ./plot t/examples/fibo.scm

then inspect the output `callgrind.out.nnnn` file using kcachegrind

    kcachegrind callgrind.out.9003


Finding potential hotspots with gprof
-------------------------------------

compile and link with -pg
run program as normal, this products a gmon.out file in current dir
run 'gprof plot' and it will generate some pretty output


Timing
------

    time ./plot t/examples/fibo.scm

consider changing CFLAGS in config.mk to include `-O3` (see commented line above CFLAGS)

pre gc:
    (fibo 31)
    2.9s
    6731342 plot_values

post gc:
    favouring arena over recycling:
        (fibo 31)
        2.3s
        4038805 plot_values

    favouring recycling over arena (correct behavior):
        (fibo 31)
        2.7s
        4038805 plot_values

        however after making many changes the figures were:
        plot GC stats:
            Max in use '4038826'
            Still had in the bank: '61174'
            num recycled '2692537', reclaimed '2692537'
            used - reclaimed '1346289'
            non-reclaimed (still in use at cleanup) '0'


        real    0m3.131s
        user    0m2.936s
        sys     0m0.188s

    after implementing collection on (temp) values generated as args
    to functions: (NB: this is the same as the previous stats +
    func-call tmp-reclamation)
        (fibo 31)
        2.9s
        plot GC stats:
            Max in use '2692574'
            Still had in the bank: '1407426'
            num recycled '4038789', reclaimed '4038805'
            used - reclaimed '-1346231'
            non-reclaimed (still in use at cleanup) '16'


        real    0m2.866s
        user    0m2.692s
        sys     0m0.164s

        This means:
            (4038826 - 2692574) / 4038826 * 100 == 33% improvement in max memory use
            (3.1 - 2.9) / 3.1 * 100 = 6.5% improvement in run-time


    implementing env memory management:
        before: 2692537 * 32 / 1024 / 1024 = 82MB
        after:       31 * 32 = 992 Bytes

        this should be freeing more values, need to look into that.
        sadly this has also increased runtime:
            plot GC stats:
            ##### plot_value stats #####
                Max in use '2692574'
                Still had in the bank: '7426'
                num recycled '4038789', reclaimed '4038805'
                used - reclaimed = '-1346231'
                non-reclaimed (still in use at cleanup) '16'
            ##### plot_env stats #####
                Max in use '31'
                Still had in the bank: '2699969'
                num recycled '2692507', reclaimed '2692537'
                used - reclaimed = '-2692506'
                non-reclaimed (still in use at cleanup) '30'


            real    0m3.056s
            user    0m2.896s
            sys     0m0.148s

    implementing correct handling of values in define and lambda:
        before: 2692574 * 48 / 1024 / 1024 = 123MB
        after:       52 * 48 / 1024 = 2.5KB

        plot GC stats:
        ##### plot_value stats #####
            Max in use '52'
            Still had in the bank: '948'
            num recycled '6731311', reclaimed '6731341'
            used - reclaimed = '-6731289'
            non-reclaimed (still in use at cleanup) '22'
        ##### plot_env stats #####
            Max in use '31'
            Still had in the bank: '969'
            num recycled '2692507', reclaimed '2692537'
            used - reclaimed = '-2692506'
            non-reclaimed (still in use at cleanup) '1'


        real    0m2.895s
        user    0m2.856s
        sys     0m0.032s

    added arena allocation for plot_hash_entry(s):
        memory usage (total) : 100MB

        plot GC stats:
        ##### plot_value stats #####
            Max in use '52'
            Still had in the bank: '948'
            num recycled '6731311', reclaimed '6731341'
            used - reclaimed = '-6731289'
            non-reclaimed (still in use at cleanup) '22'
        ##### plot_env stats #####
            Max in use '31'
            Still had in the bank: '969'
            num recycled '2692507', reclaimed '2692537'
            used - reclaimed = '-2692506'
            non-reclaimed (still in use at cleanup) '1'
        ##### plot_hash_entry stats #####
            Max in use '2692556'
            Still had in the bank: '7444'
            num recycled '0', reclaimed '0'
            used - reclaimed = '2692556'
            non-reclaimed (still in use at cleanup) '2692556'


        real    0m3.240s
        user    0m3.200s
        sys     0m0.032s

    correctly decr hash_entry(s):

        memory usage (total): 132 KB

        plot GC stats:
        ##### plot_value stats #####
            Max in use '52'
            Still had in the bank: '948'
            num recycled '6731311', reclaimed '6731341'
            used - reclaimed = '-6731289'
            non-reclaimed (still in use at cleanup) '22'
        ##### plot_env stats #####
            Max in use '31'
            Still had in the bank: '969'
            num recycled '2692507', reclaimed '2692537'
            used - reclaimed = '-2692506'
            non-reclaimed (still in use at cleanup) '1'
        ##### plot_hash_entry stats #####
            Max in use '49'
            Still had in the bank: '951'
            num recycled '2692507', reclaimed '2692537'
            used - reclaimed = '-2692488'
            non-reclaimed (still in use at cleanup) '19'


        real    0m2.809s
        user    0m2.804s
        sys     0m0.000s

    before:

        ##### plot_value stats #####
            Max in use '52'
            Still had in the bank: '948'
            num recycled '6731311', reclaimed '6731341'
            used - reclaimed = '-6731289'
            non-reclaimed (still in use at cleanup) '22'
        ##### plot_env stats #####
            Max in use '31'
            Still had in the bank: '969'
            num recycled '2692507', reclaimed '2692537'
            used - reclaimed = '-2692506'
            non-reclaimed (still in use at cleanup) '1'
        ##### plot_hash_entry stats #####
            Max in use '49'
            Still had in the bank: '951'
            num recycled '2692507', reclaimed '2692537'
            used - reclaimed = '-2692488'
            non-reclaimed (still in use at cleanup) '19'


        real    0m3.706s
        user    0m3.696s
        sys     0m0.004s


    removing subform tests from plot_eval_func_call, instead calling
    plot_eval_expr:

        plot GC stats:
        ##### plot_value stats #####
            Max in use '52'
            Still had in the bank: '948'
            num recycled '6731311', reclaimed '6731341'
            used - reclaimed = '-6731289'
            non-reclaimed (still in use at cleanup) '22'
        ##### plot_env stats #####
            Max in use '31'
            Still had in the bank: '969'
            num recycled '2692507', reclaimed '2692537'
            used - reclaimed = '-2692506'
            non-reclaimed (still in use at cleanup) '1'
        ##### plot_hash_entry stats #####
            Max in use '49'
            Still had in the bank: '951'
            num recycled '2692507', reclaimed '2692537'
            used - reclaimed = '-2692488'
            non-reclaimed (still in use at cleanup) '19'


        real    0m3.089s
        user    0m3.080s
        sys     0m0.000s


    After adding a gazilion features:
        1346269
        plot GC stats:
        ##### plot_value stats #####
            Max in use '82'
            Still had in the bank: '918'
            num recycled '9423820', reclaimed '9423880'
            used - reclaimed = '-9423798'
            non-reclaimed (still in use at cleanup) '22'
        ##### plot_env stats #####
            Max in use '31'
            Still had in the bank: '969'
            num recycled '2692507', reclaimed '2692537'
            used - reclaimed = '-2692506'
            non-reclaimed (still in use at cleanup) '1'
        ##### plot_hash_entry stats #####
            Max in use '97'
            Still had in the bank: '903'
            num recycled '2692507', reclaimed '2692537'
            used - reclaimed = '-2692440'
            non-reclaimed (still in use at cleanup) '67'


        real	0m3.793s
        user	0m3.780s
        sys	    0m0.008s

    same as above but compiled with -O3:
        1346269
        plot GC stats:
        ##### plot_value stats #####
            Max in use '82'
            Still had in the bank: '918'
            num recycled '9423820', reclaimed '9423880'
            used - reclaimed = '-9423798'
            non-reclaimed (still in use at cleanup) '22'
        ##### plot_env stats #####
            Max in use '31'
            Still had in the bank: '969'
            num recycled '2692507', reclaimed '2692537'
            used - reclaimed = '-2692506'
            non-reclaimed (still in use at cleanup) '1'
        ##### plot_hash_entry stats #####
            Max in use '97'
            Still had in the bank: '903'
            num recycled '2692507', reclaimed '2692537'
            used - reclaimed = '-2692440'
            non-reclaimed (still in use at cleanup) '67'


        real    0m3.146s
        user    0m3.140s
        sys     0m0.000s


    After combining is_form and eval_form, adding hashing and using it for eval_form
    NB: not yet using it within hash or env:
        1346269
        plot GC stats:
        ##### plot_value stats #####
            Max in use '52'
            Still had in the bank: '948'
            num recycled '6731313', reclaimed '6731343'
            used - reclaimed = '-6731291'
            non-reclaimed (still in use at cleanup) '22'
        ##### plot_env stats #####
            Max in use '31'
            Still had in the bank: '969'
            num recycled '2692507', reclaimed '2692537'
            used - reclaimed = '-2692506'
            non-reclaimed (still in use at cleanup) '1'
        ##### plot_hash_entry stats #####
            Max in use '97'
            Still had in the bank: '903'
            num recycled '2692507', reclaimed '2692537'
            used - reclaimed = '-2692440'
            non-reclaimed (still in use at cleanup) '67'


        real    0m3.727s
        user    0m3.712s
        sys     0m0.008s
    NB: our max in use has gone back down, this is due to me previously allocating a plot_value
    at the start of plot_eval_form, sometimes simply to throw it away (on return = stack unwinding).

    same as above but compiled with -O3:
        1346269
        plot GC stats:
        ##### plot_value stats #####
            Max in use '52'
            Still had in the bank: '948'
            num recycled '6731313', reclaimed '6731343'
            used - reclaimed = '-6731291'
            non-reclaimed (still in use at cleanup) '22'
        ##### plot_env stats #####
            Max in use '31'
            Still had in the bank: '969'
            num recycled '2692507', reclaimed '2692537'
            used - reclaimed = '-2692506'
            non-reclaimed (still in use at cleanup) '1'
        ##### plot_hash_entry stats #####
            Max in use '97'
            Still had in the bank: '903'
            num recycled '2692507', reclaimed '2692537'
            used - reclaimed = '-2692440'
            non-reclaimed (still in use at cleanup) '67'


        real    0m2.063s
        user    0m2.052s
        sys     0m0.004s
    NB: without -O3 the above hashing change made no difference, with -O3 it is down from
    3.2 to 2.1, that is an awesome improvement.

    after making plot_hash_get use the hash (NB: set does not yet):
        real    0m2.903s
        user    0m2.896s
        sys     0m0.000s

    same with -O3:
        real    0m1.396s
        user    0m1.388s
        sys     0m0.004s
    NB: serious improvement again from pre-hashing-o3 to post-hashing-o3
    still have to make plot_hash_set use hashes

    moving plot_hash_set to our new scheme
        real    0m3.929s
        user    0m3.920s
        sys     0m0.000s
    unexpected! huge slowdown!

    same as above with with -O3
        real    0m1.964s
        user    0m1.960s
        sys     0m0.000s
    I cannot explain this, wow.

    FIXME: unit tests give a hint:
        t/test_hash.c:43:F:test_hash:test_hash:0: Assertion 'hash.n_elems == 3' failed

    it may be my usage in _set has an error :)

    PANIC OVER, above was due to inefficient search inside get (not stopping early)
    and due to signed long long overflow causing us to stop either way too early or not early enough.
    Both have been fixed and pushed to current master
        real    0m2.604s
        user    0m2.592s
        sys     0m0.000s

    with -O3:
        real    0m1.259s
        user    0m1.252s
        sys     0m0.000s
    MUCH BETTER!!


