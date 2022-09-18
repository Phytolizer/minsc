#!/bin/sh

set -e

pushd "$(dirname "$0")"

options=$(getopt -l "help,no-codechecker,no-infer" -o "hci" -- "$@")

eval set -- "$options"

show_help() {
    cat <<EOF
Usage: $0 [options]
Options:
    -h, --help          Show this help message
    -c, --no-codechecker
                        Do not run CodeChecker
    -i, --no-infer      Do not run Infer
EOF
}

while true; do
    case "$1" in
    -h | --help)
        show_help
        exit 0
        ;;
    -c | --no-codechecker)
        NOCODECHECKER=1
        shift
        ;;
    -i | --no-infer)
        NOINFER=1
        shift
        ;;
    --)
        shift
        break
        ;;
    *)
        echo "Internal error!"
        exit 1
        ;;
    esac
done

set -x

if [ -z "$NOCODECHECKER" ]; then
    CodeChecker analyze --config codechecker.json --ignore codechecker-skip.txt -o .codechecker/reports out/build/dev-sccache-san/compile_commands.json
    (
        set +e
        CodeChecker parse -e html -o .codechecker/reports .codechecker/reports
        if [ $? -eq 1 ]; then
            echo "CodeChecker error!"
            exit 1
        fi
    )
fi
if [ -z "$NOINFER" ]; then
    infer run \
        --compilation-database out/build/dev-sccache-san/compile_commands.json \
        --skip-analysis-in-path deps/linenoise
fi

popd
