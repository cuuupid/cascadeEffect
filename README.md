<h1>READ</h1>
<ol>
<li>How to push</li>
<li>Current file resolutions</li>
</ol>

<b>How to <h6>push changes</h6> to this repository using Git + Cmd</b>

<ul>
<li>Open command prompt</li>
<li>use dir to list directories and cd "directoryName" to get inside the github repository (FTCRobotics)</li>
<li>type the following:</li>
<code>git add --all</code>
<p></p>
<code>git commit --all</code>
<p></p>
<b>Now type your changes</b>
<p>When done press Ctrl+C</p>
<code>:wq</code>
<p></p>
<code>git push --all</code>
<p></p>
<b>Type your credentials and you are set</b>


<b>Current file resolutions</b>

Currently we have many files. Multiples/duplicates everywhere. Some cleaning has been done on a fork, but this is still a junkyard of code.

TeleOp_Meca under cascade effect is what must be used. It's includes are everywhere. For simpler usage ask Priansh for forked repo.
CavalierC is a compilation of brixcc utils. 
Lift.c is deprecated, liftFunctions.c is gunner, button presets are in joyFunctions.c
