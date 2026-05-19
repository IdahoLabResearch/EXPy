---
name: tdd
description: Test-driven development with red-green-refactor loop. Use when user wants to build features or fix bugs using TDD, mentions "red-green-refactor", wants integration tests, or asks for test-first development.
---

# Test-Driven Development

## Philosophy

**Core principle**: Tests should verify behavior through public interfaces, not implementation details. Code can change entirely; tests shouldn't.

**Good tests** are integration-style: they exercise real code paths through public APIs. They describe _what_ the system does, not _how_ it does it. A good test reads like a specification - "user can checkout with valid cart" tells you exactly what capability exists. These tests survive refactors because they don't care about internal structure.

**Bad tests** are coupled to implementation. They mock internal collaborators, test private methods, or verify through external means (like querying a database directly instead of using the interface). The warning sign: your test breaks when you refactor, but behavior hasn't changed. If you rename an internal function and tests fail, those tests were testing implementation, not behavior.

See [tests.md](tests.md) for examples and [mocking.md](mocking.md) for mocking guidelines.

## Anti-Pattern: Horizontal Slices

**DO NOT write all tests first, then all implementation.** This is "horizontal slicing" - treating RED as "write all tests" and GREEN as "write all code."

This produces **crap tests**:

- Tests written in bulk test _imagined_ behavior, not _actual_ behavior
- You end up testing the _shape_ of things (data structures, function signatures) rather than user-facing behavior
- Tests become insensitive to real changes - they pass when behavior breaks, fail when behavior is fine
- You outrun your headlights, committing to test structure before understanding the implementation

**Correct approach**: Vertical slices via tracer bullets. One test → one implementation → repeat. Each test responds to what you learned from the previous cycle. Because you just wrote the code, you know exactly what behavior matters and how to verify it.

```
WRONG (horizontal):
  RED:   test1, test2, test3, test4, test5
  GREEN: impl1, impl2, impl3, impl4, impl5

RIGHT (vertical):
  RED→GREEN: test1→impl1
  RED→GREEN: test2→impl2
  RED→GREEN: test3→impl3
  ...
```

## Workflow

### 0. Detect Existing Test Infrastructure

Before writing any test, **probe what's already there**. Do NOT scaffold a parallel suite — that has burned this project before (two suites, neither runnable end-to-end).

- [ ] Check for a runner: `Makefile` (look for a `test`/`check` target), `pyproject.toml` / `pytest.ini` / `tox.ini`, `package.json` (`scripts.test`), `Cargo.toml`, `go.mod`, etc.
- [ ] Try the canonical command (`make test`, `pytest`, `npm test`, `cargo test`, `go test ./...`) and confirm it actually runs and collects tests.
- [ ] If the command exists but the environment is broken (e.g. `pytest: No such file or directory`), fix the **environment** — install deps, activate the venv, update the Makefile target — before adding tests. Do not sidestep by creating a parallel suite.
- [ ] Match the existing layout: test directory location, file naming (`test_*.py` vs `*_test.py`), fixture style, assertion library. New tests must land in the same suite the team already runs.
- [ ] If no infrastructure exists at all, **stop and ask the user** which framework and layout to use. Do not pick unilaterally.

A second test suite that doesn't integrate is worse than no new tests. If you cannot find or cannot run the existing suite, surface that to the user before proceeding.

### 1. Planning

When exploring the codebase, use the project's domain glossary so that test names and interface vocabulary match the project's language, and respect ADRs in the area you're touching.

Before writing any code:

- [ ] Confirm with user what interface changes are needed
- [ ] Confirm with user which behaviors to test (prioritize)
- [ ] Identify opportunities for [deep modules](deep-modules.md) (small interface, deep implementation)
- [ ] Design interfaces for [testability](interface-design.md)
- [ ] List the behaviors to test (not implementation steps)
- [ ] Get user approval on the plan

Ask: "What should the public interface look like? Which behaviors are most important to test?"

**You can't test everything.** Confirm with the user exactly which behaviors matter most. Focus testing effort on critical paths and complex logic, not every possible edge case.

### 2. Tracer Bullet

Write ONE test that confirms ONE thing about the system:

```
RED:   Write test for first behavior → test fails
GREEN: Write minimal code to pass → test passes
```

This is your tracer bullet - proves the path works end-to-end.

### 3. Incremental Loop

For each remaining behavior:

```
RED:   Write next test → fails
GREEN: Minimal code to pass → passes
```

Rules:

- One test at a time
- Only enough code to pass current test
- Don't anticipate future tests
- Keep tests focused on observable behavior

### 4. Refactor

After all tests pass, look for [refactor candidates](refactoring.md):

- [ ] Extract duplication
- [ ] Deepen modules (move complexity behind simple interfaces)
- [ ] Apply SOLID principles where natural
- [ ] Consider what new code reveals about existing code
- [ ] Run tests after each refactor step

**Never refactor while RED.** Get to GREEN first.

### 5. Wrap up: commit and close the issue

When the user signals the work is done — "looks good", "ship it", "close the issue", "mark this resolved", or otherwise approves — **commit the changes and close the linked GitHub issue in the same step**. Do not wait for them to ask again.

- [ ] Identify the issue number. If `/tdd` was invoked with a specific issue (e.g. "work on #9"), use that. If the agent picked from the open queue ("next open issue"), it must remember which number it picked and surface it before this step.
- [ ] `git status` to confirm what actually changed. Stage only the files that belong to this issue — do not sweep in unrelated edits.
- [ ] Commit with a message that references the issue and follows the project's log style (look at `git log --oneline -10` for the cadence). Include a `Closes #N` trailer so GitHub will auto-link.
- [ ] If pre-commit hooks fail, fix the underlying issue and create a **new** commit. Do not `--amend` and do not `--no-verify`.
- [ ] Close the issue via `gh issue close <N> --comment "<one-line summary of what landed and the commit SHA>"`. The comment matters — future-you reading the closed issue should see what shipped without having to dig.
- [ ] Do **not** push without asking. Surface the commit SHA and ask "push to origin?" before doing it.
- [ ] If the user has not explicitly approved (they only said "tests pass" or you decided GREEN was enough), **do not** commit or close. Ask: "Tests are green and I refactored — ready to commit and close #N?"

This step is mandatory at the end of every `/tdd` cycle that began with a tracked issue. Skipping it leaves the issue queue out of sync with the code and forces the next `/tdd` session to re-discover state.

## Checklist Per Cycle

```
[ ] Test describes behavior, not implementation
[ ] Test uses public interface only
[ ] Test would survive internal refactor
[ ] Code is minimal for this test
[ ] No speculative features added
```
