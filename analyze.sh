#!/bin/sh

set -xe

CodeChecker analyze --config codechecker.json --ignore codechecker-skip.txt -o .codechecker/reports out/build/dev-sccache-san/compile_commands.json
(
    set +e
    CodeChecker parse -e html -o .codechecker/reports .codechecker/reports
    if [ $? -eq 1 ]; then
        echo "CodeChecker error!"
        exit 1
    fi
)
infer run --compilation-database out/build/dev-sccache-san/compile_commands.json --skip-analysis-in-path deps/linenoise
