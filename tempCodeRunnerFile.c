int flags = fcntl(s, F_GETFL);
    // flags |= O_NONBLOCK;
    // fcntl(s, F_SETFL, flags);