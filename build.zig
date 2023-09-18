const std = @import("std");
// import the dependency
const zcc = @import("compile_commands");

pub fn build(b: *std.Build) !void {
    // make a list of targets that have include files and c source files
    var targets = std.ArrayList(*std.Build.CompileStep).init(b.allocator);

    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const lib = b.addStaticLibrary(.{
        .root_source_file = .{ .path = "src/str.c" },
        .name = "str",
        .optimize = optimize,
        .target = target,
    });
    lib.addCSourceFile(.{ .file = .{ .path = "src/fmt.c" }, .flags = &.{} });
    targets.append(lib) catch @panic("OOM");
    lib.linkLibC();
    lib.addIncludePath(.{ .path = "include" });
    b.installArtifact(lib);

    const ctest = b.addExecutable(.{
        .name = "ctest",
        .root_source_file = .{ .path = "test/main.c" },
        .optimize = optimize,
        .target = target,
    });
    targets.append(ctest) catch @panic("OOM");
    ctest.linkLibC();
    ctest.linkLibrary(lib);
    ctest.addIncludePath(.{ .path = "include" });
    ctest.step.dependOn(b.getInstallStep());

    const run_test = b.addRunArtifact(ctest);
    run_test.step.dependOn(b.getInstallStep());

    const test_step = b.step("test", "test the lib");
    test_step.dependOn(&run_test.step);

    // add a step called "cdb" (Compile commands DataBase) for making
    // compile_commands.json. could be named anything. cdb is just quick to type
    zcc.createStep(b, "cdb", try targets.toOwnedSlice());
}
