{
    # compiler settings to build the nanomsg library
    'defines': [
        'MILL_KQUEUE',
    ],
    'direct_dependent_settings': {
        # build nanomsg hub with same compiler flags as the library
        'defines': [
            'MILL_KQUEUE',
        ],
    }
}
