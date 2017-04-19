/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.ther;

import com.google.common.base.MoreObjects;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.block.BlockBorder;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.AbstractXYItemRenderer;
import org.jfree.chart.renderer.xy.XYStepRenderer;
import org.jfree.data.xy.XYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

public class TempLevelSim {

    private static final int PROBES = 500;
    final Driver driver;
    private float currentTemp = 50;
    private static final float TEMP_INCREASE = 1.5f;
    private static final float TEMP_DECREASE = -0.5f;
    private static final float TEMP_SETPOINT = 30;
    private static final float TEMP_MIN = 20;

    public TempLevelSim(Driver driver) {
        this.driver = driver;
    }

    public static void main(String... par) {
        new TempLevelSim(new PIDDriver()).simulate();
    }

    public void simulate() {
        List<ProbePoint> probes = new ArrayList<>(PROBES);
        for (int sample = 0; sample < PROBES; sample++) {

            PIDVal pidVal = driver.drive(TEMP_SETPOINT, currentTemp);
            simulateTempChange(pidVal.relay);
            probes.add(new ProbePoint(sample, pidVal));

            System.out.println(sample + " -> " + pidVal);
        }

        ChartUI chartUI = new ChartUI();
        chartUI.createUI(probes);
        chartUI.setVisible(true);
    }

    private void simulateTempChange(RelaySwitch relaySwitch) {
        if (relaySwitch == RelaySwitch.ON) {
            currentTemp += TEMP_DECREASE;
        } else {
            currentTemp += TEMP_INCREASE;
        }

        if(currentTemp < TEMP_MIN){
            currentTemp = TEMP_MIN;
        }
    }

    enum RelaySwitch {
        ON, OFF
    }


    /**
     * 1) p=1,i=2 - slow cooling, less relay switches, temp not accurate
     * 2) p=6,i=2 - aggressive cooling, frequent relay switches, temp accurate
     */
    static class PIDDriver implements Driver {
        private static final float AMP_P = 1;
        private static final float AMP_I = 2f;
        private static final float AMP_D = 0f;
        private static final float SWITCH_THRESHOLD = -10.0f;
        private float iDerivationSum = 0;
        private float dPrevDerivation = 0;


        private float calculateP(float derivation) {
            return AMP_P * derivation;
        }

        private float calculateI(float derivation) {
            iDerivationSum += derivation;
            return AMP_I * iDerivationSum;
        }

        private float calculateD(float derivation) {
            float d = AMP_D * (derivation - dPrevDerivation);
            dPrevDerivation = derivation;
            return d;
        }

        @Override
        public PIDVal drive(float setPointTemp, float currentTemp) {
            float derivation = setPointTemp - currentTemp;
            float valP = calculateP(derivation);
            float valI = calculateI(derivation);
            float valD = calculateD(derivation);

            float pid = valP + valI + valD;

            // negative PID means cooling ON
            RelaySwitch relay = pid < SWITCH_THRESHOLD ? RelaySwitch.ON : RelaySwitch.OFF;

            return new PIDVal(valP, valI, valD, pid, setPointTemp, currentTemp, relay);
        }
    }


    static class ProbePoint {
        final int sample;
        final PIDVal pidVal;

        ProbePoint(int sample, PIDVal pidVal) {
            this.sample = sample;
            this.pidVal = pidVal;
        }
    }

    interface Driver {
        PIDVal drive(float setPointTemp, float currentTemp);
    }


    static class PIDVal implements Cloneable {
        final float valP;
        final float valI;
        final float valD;
        final float pid;
        final float setPointTemp;
        final float currentTemp;
        final RelaySwitch relay;

        public PIDVal(float valP, float valI, float valD, float pid, float setPointTemp, float currentTemp, RelaySwitch relay) {
            this.valP = valP;
            this.valI = valI;
            this.valD = valD;
            this.pid = pid;
            this.setPointTemp = setPointTemp;
            this.currentTemp = currentTemp;
            this.relay = relay;
        }

        @Override
        protected PIDVal clone() {
            return new PIDVal(valP, valI, valD, pid, setPointTemp, currentTemp, relay);
        }

        @Override
        public String toString() {
            return MoreObjects.toStringHelper("PID")
                    .add("valP", valP)
                    .add("valI", valI)
                    .add("valD", valD)
                    .add("pid", pid)
                    .add("setPointTemp", setPointTemp)
                    .add("currentTemp", currentTemp)
                    .add("relay", relay)

                    .toString();
        }
    }

    static class ChartUI extends JFrame {

        private final static String TITLE = "Temp level change in time";

        private XYDataset createDataset(List<ProbePoint> probes) {
            XYSeries seriesTemp = new XYSeries("temp");
            XYSeries seriesSp = new XYSeries("SP");
            XYSeries seriesRelay = new XYSeries("relay");

            XYSeries seriesP = new XYSeries("P");
            XYSeries seriesI = new XYSeries("I");
            XYSeries seriesD = new XYSeries("D");
           XYSeries seriesPID = new XYSeries("PID");


            for (ProbePoint point : probes) {
                int sample = point.sample;
                seriesTemp.add(sample, point.pidVal.currentTemp);
                seriesSp.add(sample, point.pidVal.setPointTemp);
                seriesRelay.add(sample, point.pidVal.relay == RelaySwitch.ON ? 10 : 0);
                seriesP.add(sample, point.pidVal.valP);
                seriesI.add(sample, point.pidVal.valI);
                seriesD.add(sample, point.pidVal.valD);
                seriesPID.add(sample, point.pidVal.pid);
            }
            XYSeriesCollection dataset = new XYSeriesCollection();
            dataset.addSeries(seriesTemp);
            dataset.addSeries(seriesSp);
            dataset.addSeries(seriesRelay);

            dataset.addSeries(seriesP);
            dataset.addSeries(seriesI);
           // dataset.addSeries(seriesD);
            dataset.addSeries(seriesPID);
            return dataset;
        }

        private JFreeChart createChart(XYDataset dataset) {

            JFreeChart chart = ChartFactory.createXYLineChart(
                    TITLE,
                    "Time",
                    "Level",
                    dataset,
                    PlotOrientation.VERTICAL,
                    true,
                    false,
                    false
            );

            AbstractXYItemRenderer renderer = new XYStepRenderer();

            renderer.setSeriesPaint(0, Color.RED);
            renderer.setSeriesStroke(0, createStroke());

            renderer.setSeriesPaint(1, Color.BLUE);
            renderer.setSeriesStroke(1, createStroke());

            renderer.setSeriesPaint(2, Color.GREEN);
            renderer.setSeriesStroke(2, createStroke());

            renderer.setSeriesPaint(3, Color.PINK);
            renderer.setSeriesStroke(3, createStroke());

            renderer.setSeriesPaint(4, Color.WHITE);
            renderer.setSeriesStroke(4, createStroke());

            renderer.setSeriesPaint(5, Color.BLACK);
            renderer.setSeriesStroke(5, createStroke());

            renderer.setSeriesPaint(6, Color.YELLOW);
            renderer.setSeriesStroke(6, createStroke());

            XYPlot plot = chart.getXYPlot();
            plot.setRenderer(renderer);
            plot.setBackgroundPaint(Color.GRAY);

            chart.getLegend().setFrame(BlockBorder.NONE);
            return chart;
        }

        private Stroke createStroke() {
            return new BasicStroke();
        }

        private void createUI(List<ProbePoint> probes) {
            XYDataset dataset = createDataset(probes);
            JFreeChart chart = createChart(dataset);

            ChartPanel chartPanel = new ChartPanel(chart);
            chartPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
            chartPanel.setBackground(Color.BLACK);
            add(chartPanel);

            pack();
            setTitle(TITLE);
            setLocationRelativeTo(null);
            setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        }
    }
}
