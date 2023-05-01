
class FileSystem {
public:
    FileSystem() {}
    int numDisks() const { return 1; }
};

extern FileSystem tfs; // 可能定义在另一个源文件内, 初始化顺序影响对象构造

FileSystem& tfs1() {
    static FileSystem fs;
    return fs;
}

class Directory {
public:
    Directory();
};

Directory::Directory() {
    // int disks = tfs.numDisks();     //
    int disks1 = tfs1().numDisks(); //
}

// 返回引用: 多线程安全(防止竞态条件)
Directory& tempDir1() {
    static Directory td;
    return td;
}

void t1() {
    Directory tempDir;
    // 需要考虑初始化顺序问题
}

void t2() {
    Directory tmp = tempDir1(); //
}

int main(int argc, char const* argv[]) {
    t1();
    return 0;
}