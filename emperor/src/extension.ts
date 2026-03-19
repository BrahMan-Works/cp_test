import * as vscode from 'vscode';
import { exec } from 'child_process';
import * as fs from 'fs';

export function activate(context: vscode.ExtensionContext) {

    let disposable = vscode.commands.registerCommand('cp-test.run', async () => {

        const input = await vscode.window.showInputBox({ prompt: "Paste Input" });
        const output = await vscode.window.showInputBox({ prompt: "Paste Expected Output" });

        if (!input) return;

        fs.mkdirSync(".cp_test/tests", { recursive: true });

        fs.writeFileSync(".cp_test/tests/1.in", input);
        fs.writeFileSync(".cp_test/tests/1.out", output || "");

        exec("cp_test main.cpp", (err, stdout, stderr) => {
            vscode.window.showInformationMessage(stdout || stderr);
        });
    });

    context.subscriptions.push(disposable);
}

export function deactivate() {}
