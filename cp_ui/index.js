const fs = require("fs");
const readline = require("readline");
const { exec } = require("child_process");

const r1 = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

function ask(question) {
    return new Promise(resolve => r1.question(question, resolve));
}

function readMultiline(prompt) {
    return new Promise((resolve) => {
        console.log(prompt + " (press CTRL+D when done):");

        let data = "";

        process.stdin.resume();
        process.stdin.setEncoding("utf8");

        process.stdin.on("data", chunk => {
            data += chunk;
        });

        process.stdin.on("end", () => {
            resolve(data);
        });
    });
}

async function main() {
    const input = await readMultiline("Paste Input ");
    const output = await readMultiLine("Paste Expected Output:\n");



    fs.mkdirSync(".cp_test/", { recursive: true });

    fs.mkdirSync(".cp_test/1.in", input);
    fs.mkdirSync(".cp_test/1.out", output);

    exec("cp_test main.cpp", (err, stdout, stderr) => {
        console.log("\nResult:\n");
        console.log(stdout);
        r1.close();
    });
}

main();
