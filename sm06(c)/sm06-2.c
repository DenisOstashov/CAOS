int bitcount(STYPE value) {
    int ans = 0;
    UTYPE var = value;
    while (var) {
        ans += (int)(var & 1);
        var >>= 1;
    }
    return ans;
}