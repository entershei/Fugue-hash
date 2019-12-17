#ifndef FUGUEHASH_TABLES_H
#define FUGUEHASH_TABLES_H


std::vector<uint32_t> IV = {3914513886, 1718686559, 3772053096, 3534796180, 4184629789, 4227410398, 2437539993,
                            888717896};


std::vector<std::vector<uint8_t >> M = {{1, 4, 7, 1},
                              {1, 1, 4, 7},
                              {7, 1, 1, 4},
                              {4, 7, 1, 1}};

std::vector<std::vector<uint8_t >> M_t = {{1, 1, 7, 4},
                                          {4, 1, 1, 7},
                                          {7, 4, 1, 1},
                                          {1, 7, 4, 1}};

#endif //FUGUEHASH_TABLES_H
